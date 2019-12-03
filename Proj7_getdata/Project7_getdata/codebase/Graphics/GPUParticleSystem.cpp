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
#include "Graphics/GPUParticleSystem.h"
#include "System/Renderer.h"
#include "Component/ParticleComponent.h"
#include "Entity.h"
#include "Enums.h"

#define MAX_GPU_PARTICLE_NUM 512
#define MAX_GPU_EMITTER_NUM 256
#define WORK_GROUP_SIZE 256
#define PI 3.1415926

C_GPUParticleSystem::C_GPUParticleSystem(C_Renderer & _renderer) : renderer(_renderer), particleEmmiterLifetimes(MAX_GPU_EMITTER_NUM),particleEmitterSpeeds(MAX_GPU_EMITTER_NUM), particleEmitRanges(MAX_GPU_EMITTER_NUM), particleWorkingRanges(MAX_GPU_EMITTER_NUM), particleHandleManager(MAX_GPU_EMITTER_NUM - 1), startPositions(MAX_GPU_EMITTER_NUM)
{

}

C_GPUParticleSystem::~C_GPUParticleSystem()
{
	GLuint buffers[] = { particleWorkingRangeBuffer ,particleStartPositionBuffer, particlePositionBuffer ,particleVelocityBuffer };
	glDeleteBuffers(4, buffers);
}

void C_GPUParticleSystem::Init()
{
	glGenBuffers(1, &particleWorkingRangeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleWorkingRangeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, 2 * MAX_GPU_EMITTER_NUM * sizeof(unsigned int), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleStartPositionBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleStartPositionBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_EMITTER_NUM * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleEmitterLifetimeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitterLifetimeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_EMITTER_NUM * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleEmitterSpeedBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitterSpeedBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_EMITTER_NUM * sizeof(vec2), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleEmitRangeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitRangeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_EMITTER_NUM * sizeof(vec2), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleLifetimeBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleLifetimeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_PARTICLE_NUM * MAX_GPU_EMITTER_NUM * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particlePositionBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particlePositionBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_PARTICLE_NUM * MAX_GPU_EMITTER_NUM * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	glGenBuffers(1, &particleVelocityBuffer);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleVelocityBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, MAX_GPU_PARTICLE_NUM * MAX_GPU_EMITTER_NUM * sizeof(vec3), NULL, GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

	GLint compile_result;
	std::string shader_source = file_func::LoadTextFile(file_func::GetPath("Graphics/Shaders/BasicParticle2D.comp").c_str());

	const char* shader_cstr = shader_source.c_str();
	GLuint shaderHandle = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(shaderHandle, 1, &shader_cstr, 0);
	glCompileShader(shaderHandle);
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compile_result);
	std::string type_name = "Compute Shader";
	if (!compile_result)
	{
		char buffer[1024];
		glGetShaderInfoLog(shaderHandle, 1024, 0, buffer);
		debug_func::ReportErr(type_name + " shader creation failed!:\t" + std::string(&buffer[0]));
	}

	particleComputePipeline = glCreateProgram();
	glAttachShader(particleComputePipeline, shaderHandle);
	glLinkProgram(particleComputePipeline);
	GLint link_result;
	glGetProgramiv(particleComputePipeline, GL_LINK_STATUS, &link_result);
	if (!link_result) {
		char buffer[1024];
		glGetProgramInfoLog(particleComputePipeline, 1024, 0, buffer);
		debug_func::ReportErr("Pipeline linking failed!:\t" + std::string(&buffer[0]));
	}
	view_loc = glGetUniformLocation(particleComputePipeline, "view");
	proj_loc = glGetUniformLocation(particleComputePipeline, "proj");
	tex_loc = glGetUniformLocation(particleComputePipeline, "DepthInfo");
	dt_time_loc = glGetUniformLocation(particleComputePipeline, "deltaTime");
	particle_size_loc = glGetUniformLocation(particleComputePipeline, "ParticleSize");
}

void C_GPUParticleSystem::Update(C_World & worldname, float _delta_time)
{
	graphics::S_DepthInfo const &depth = renderer.GetDepthInfo();
	for (size_t pid : gpuParticleComponents)
	{
		C_Entity &entity = worldname.mEntities[pid];
		C_ParticleComponent &particle = worldname.mParticles[entity.mComponents[E_CP_PARTICLE]];
		C_Transform &transform_cmp = worldname.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
		startPositions[particle.GPUParticleHandle] = transform_cmp.WorldPos;// *vec4(0.0f, 0.0f, 0.0f, 1.0f);
		particleWorkingRanges[particle.GPUParticleHandle].AccHead();
		if (particle.play)
		{
			//update working range
			particleEmitterSpeeds[particle.GPUParticleHandle] = particle.initialSpeedRange;
			particle.currentInterval -= _delta_time;
			float rad_half = 0.5f * particle.directionAngleX * PI / 180.0f;
			particleEmitRanges[particle.GPUParticleHandle] = vec2(transform_cmp.GetWorldRotation2D() - rad_half, transform_cmp.GetWorldRotation2D() + rad_half);
			if (particle.currentInterval <= 0.0f)
			{
				particle.currentInterval = particle.interval;
				//spawn
				if (particle.play)
				{
					particleWorkingRanges[particle.GPUParticleHandle].AccTail(particle.spawnNumber, MAX_GPU_PARTICLE_NUM);
				}

				if (!particle.loop)
				{
					particle.play = false;
				}
			}
		}
	}
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleStartPositionBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleHandleManager.LastID * sizeof(vec3), startPositions.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleWorkingRangeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleHandleManager.LastID * sizeof(S_SliderPair), particleWorkingRanges.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitterLifetimeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleHandleManager.LastID * sizeof(GLfloat), particleEmmiterLifetimes.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitterSpeedBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleHandleManager.LastID * sizeof(vec2), particleEmitterSpeeds.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleEmitRangeBuffer);
	glBufferData(GL_SHADER_STORAGE_BUFFER, particleHandleManager.LastID * sizeof(vec2), particleEmitRanges.data(), GL_DYNAMIC_DRAW);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

 	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	glUseProgram(particleComputePipeline);

	//std::cout << _delta_time << std::endl;
	glUniform1f(dt_time_loc, _delta_time);

	glUniform1i(tex_loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, depth.depthBuffer);

	glUniformMatrix4fv(view_loc, 1, false, &(depth.view->pViewMatrix[0]));
	glUniformMatrix4fv(proj_loc, 1, false, &(depth.view->pProjectMatrix[0]));

	glUniform1i(particle_size_loc, MAX_GPU_PARTICLE_NUM);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleWorkingRangeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, particleStartPositionBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, particleVelocityBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 3, particlePositionBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 4, particleLifetimeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 5, particleEmitterLifetimeBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 6, particleEmitterSpeedBuffer);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 7, particleEmitRangeBuffer);

	glDispatchCompute(MAX_GPU_PARTICLE_NUM / WORK_GROUP_SIZE, particleHandleManager.LastID, 1);

	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT);

}

void C_GPUParticleSystem::RegisterGpuParticle(C_World & worldname, size_t _entity, size_t _particle_cmp)
{
	C_ParticleComponent &particle = worldname.mParticles[_particle_cmp];
	if (particle.GPUParticleHandle != -1)
	{
		//std::cout << _entity <<" already registered!" << std::endl;
		return;
	}
	particle.GPUParticleHandle = particleHandleManager.assign();
	particle.gpuPosInfo.buffer = particlePositionBuffer;
	particle.gpuPosInfo.offset = particle.GPUParticleHandle * MAX_GPU_PARTICLE_NUM;
	particle.gpuVelInfo.buffer = particleVelocityBuffer;
	particle.gpuVelInfo.offset = particle.GPUParticleHandle * MAX_GPU_PARTICLE_NUM;
	particle.gpuLifetimeInfo.buffer = particleLifetimeBuffer;
	particle.gpuLifetimeInfo.offset = particle.GPUParticleHandle * MAX_GPU_PARTICLE_NUM;
	particle.gpuInstNum = MAX_GPU_PARTICLE_NUM;
	particleEmmiterLifetimes[particle.GPUParticleHandle] = particle.initialLifetime;
	gpuParticleComponents.insert(_entity);
}

void C_GPUParticleSystem::UnregisterGpuParticle(C_World & worldname, size_t _entity, size_t _particle_cmp)
{
	C_ParticleComponent &particle = worldname.mParticles[_particle_cmp];
	if (particle.GPUParticleHandle == -1)
	{
		//std::cout << _particle_cmp << " in " << _entity << " already unregistered!" << std::endl;
		return;
	}
	//else
	//{
	//	std::cout << "unregister " << _particle_cmp << " in "<< _entity << std::endl;
	//}
	particleHandleManager.free(particle.GPUParticleHandle);
	//for (auto &i : particleHandleManager.available)
	//{
	//	std::cout << i << " ";
	//}
	particle.GPUParticleHandle = -1;
	//std::cout << std::endl;
	gpuParticleComponents.erase(_entity);
}

void S_SliderPair::AccTail(uint32_t acc, uint32_t _max)
{
	//uint32_t new_tail = (Tail + acc);
	//if (new_tail >= _max + Head)
	//{
	//	new_tail = _max + Head - 1;
	//}
	//new_tail = new_tail % _max;


	Tail = (Tail + acc) % _max;
}

void S_SliderPair::AccHead()
{
	Head = Tail;
	//uint32_t new_head = (Head + acc);
	//if (new_head > _max + Tail)
	//{
	//	new_head = _max + Tail;
	//}
	//new_head = new_head % _max;
	////if (((Tail >= new_head) ^ phase))
	////{
	////	phase = !phase;
	////	new_head = Tail;
	////}
	//Head = new_head;
}
