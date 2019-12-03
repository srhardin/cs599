/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Jin Fan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "System/ParticleSys.h"
#include "Singleton/ResourceManager.h"
#include "Enums.h"
#include "Entity.h"
#include "Component/ParticleComponent.h"
#include "System/Renderer.h"
#include <thread>
#include <atomic>
#include <random>
#define MULTITHREADED false
#define THREAD_NUM 1
	class C_ParticleWorkThread
	{
	public:
		C_ParticleWorkThread() : deltatime(0.0f), particles(), working(false), end(false)
		{
			std::thread work(&C_ParticleWorkThread::WorkingFunction, this);
			work.detach();
		}
		void WorkingFunction()
		{
#if MULTITHREADED
			while (!end)
			{
#endif
				if (working)
				{
					std::set<int>::iterator pitr = particles.begin();
					std::set<int>::iterator pend = particles.end();
					while (pitr != pend)
					{
						C_Entity &entity = world->mEntities[*pitr];
						C_ParticleComponent &cmp = world->mParticles[entity.mComponents[E_CP_PARTICLE]];
						if (cmp.play || cmp.number > 0)
						{
							C_Transform &transform_cmp = world->mTransforms[entity.mComponents[E_CP_TRANSFORM]];
							vec3 start_pos = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f);
							vec4 worldQ = transform_cmp.WorldQuaternion;
							//remove died particles
							size_t current_id = 0;
							while (current_id < cmp.number)
							{
								cmp.lifetime[current_id] -= deltatime;
								if (cmp.lifetime[current_id] <= 0.0f)//invalidate
								{
									cmp.positions[current_id] = cmp.positions[cmp.number - 1];
									cmp.velocities[current_id] = cmp.velocities[cmp.number - 1];
									cmp.lifetime[current_id] = cmp.lifetime[cmp.number - 1];
									--cmp.number;
								}
								else
								{
									++current_id;
								}
							}
							//update alive particles
							for (size_t i = 0; i < cmp.number; ++i)
							{
								cmp.velocities[i] += cmp.accel * deltatime;
								cmp.positions[i] += cmp.velocities[i] * deltatime;
								//float speed = glm::length(cmp.velocities[current_id]);
							}
							cmp.currentInterval -= deltatime;
							if (cmp.currentInterval <= 0.0f)
							{
								//spawn
								if (cmp.play)
								{
									size_t spawn_number = min(cmp.spawnNumber, cmp.maxNumber - cmp.number);
									//float rad_half = 0.5f * cmp.directionAngleX * 3.1415926f / 180.0f;

									float rad_half = DegreeToRadius(cmp.directionAngleX * 0.5f);
									vec2 emit_range( -rad_half, rad_half);

									for (size_t i = 0; i < spawn_number; ++i)
									{
										size_t true_id = cmp.number + i;
										cmp.lifetime[true_id] = cmp.initialLifetime;
										cmp.positions[true_id] = start_pos;
										float rd = (rand() % 1000) * 0.001f;
										float rd2 = (rand() % 1000) * 0.001f;
										float emit_dir = (emit_range.y - emit_range.x) * rd + emit_range.x;
										vec4 addQ = GetQuater(vec4(0, 0, 1.0f, emit_dir));
										vec4 outQ = QuaterMulti(addQ, worldQ);
										
										cmp.velocities[true_id] = (cmp.initialSpeedRange.x + rd2 * (cmp.initialSpeedRange.y - cmp.initialSpeedRange.x)) * RotationVector(outQ, vec3(1.0f, 0.0f, 0.0f));
									}
									cmp.number += spawn_number;
								}
								//cmp.currentInterval = 0.0f;
								//if (cmp.loop)
								//{
									cmp.currentInterval = cmp.interval;
								//}
							}
							if (!cmp.loop)
							{
								cmp.play = false;
							}
						}
						++pitr;
					}
					working = false;
				}
#if MULTITHREADED
			}
#endif
		}
		bool Done()
		{
			return !working;
		}
		void Start(float _deltatime, C_World &_world)
		{
#if MULTITHREADED
			if (Done()) 
			{
				deltatime = _deltatime;
				world = &_world;
				working = true;
			}
#else
			deltatime = _deltatime;
			world = &_world;
			working = true;
			WorkingFunction();
#endif
		}
		void End()
		{
			while (!Done()) {}
			end = true;
		}
		void RegisterParticle(int id)
		{
			particles.insert(id);
		}
		void UnregisterParticle(int id)
		{
			particles.erase(id);
		}
	private:
		float deltatime;
		C_World *world;
		std::set<int> particles;
		std::atomic<bool> working;
		std::atomic<bool> end;
		//std::atomic<bool>
	};


C_ParticleSys::C_ParticleSys() : threads(THREAD_NUM), currentThread(0), threadNum(THREAD_NUM)
{

}

C_ParticleSys::~C_ParticleSys()
{
	for (size_t i = 0; i < threadNum; ++i)
	{
		threads[i].End();
	}
}

void C_ParticleSys::Register(C_World & worldname, std::vector<int>& entityList)
{
	//C_ResourceManager &mgr = *C_ResourceManager::GetInstance();
	for (int &id : entityList)
	{
		C_Entity &entity = worldname.mEntities[id];
		if ((entity.entityMask & std::bitset<64>(E_CPM_PARTICLE)).any())
		{
			C_ParticleComponent &particle_cmp = worldname.mParticles[entity.mComponents[E_CP_PARTICLE]];
			if (!particle_cmp.runOnGPU)
			{
				threads[currentThread].RegisterParticle(id);
				currentThread = (currentThread + 1) % threadNum;
			}
		}
	}
}

void C_ParticleSys::Unregister(C_World & worldname, std::vector<int>& entityList)
{
	for (int &id : entityList)
	{
		C_Entity &entity = worldname.mEntities[id];
		if ((entity.entityMask & std::bitset<64>(E_CPM_PARTICLE)).any())
		{
			C_ParticleComponent &particle_cmp = worldname.mParticles[entity.mComponents[E_CP_PARTICLE]];
			if (!particle_cmp.runOnGPU)
			{
				for (auto &thread : threads)
				{
					thread.UnregisterParticle(id);
				}
			}
			//particles.erase(id);
		}
	}
}

void C_ParticleSys::Update(C_World & worldname, float deltaTime)
{
	if (C_Renderer::globalRenderer->GetOutputWindow()->Activated) 
	{
		for (size_t i = 0; i < threadNum; ++i)
		{
			threads[i].Start(deltaTime, worldname);
		}
		for (size_t i = 0; i < threadNum; ++i)
		{
			while (!threads[i].Done()) {}
		}
	}
}
