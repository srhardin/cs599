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
#include "Component/ParticleComponent.h"
#include "System/Renderer.h"

C_ParticleComponent::C_ParticleComponent()
{
	Initialize();
}

void C_ParticleComponent::Serialize(S_JsonValue & val)
{
	C_Renderer *renderer = C_Renderer::globalRenderer;
	bool support_gpu = renderer->SupportComputeShader();
	std::vector<std::string> names = val.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "max_num")
		{
			maxNumber = val[itr].asInt();
		}
		else if (itr == "spawn_num")
		{
			spawnNumber = val[itr].asInt();
		}
		else if (itr == "life_time")
		{
			initialLifetime = val[itr].asFloat();
		}
		else if (itr == "interval")
		{
			interval = val[itr].asFloat();
		}
		else if (itr == "loop")
		{
			loop = val[itr].asBool();
		}
		else if (itr == "play")
		{
			play = val[itr].asBool();
		}
		else if (itr == "speed")
		{
			initialSpeedRange = val[itr].asVec2();
		}
		else if (itr == "accel")
		{
			accel = val[itr].asVec3();
		}
		else if (itr == "angle")
		{
			directionAngleX = val[itr].asFloat();
		}
		else if (itr == "gpu")
		{
			runOnGPU = val[itr].asBool() && support_gpu;
		}
		else if (itr == "size")
		{
			size = val[itr].asVec2();
		}
	}
	positions.resize(maxNumber);
	velocities.resize(maxNumber);
	lifetime.resize(maxNumber);
}

void C_ParticleComponent::Initialize()
{
	maxNumber = 100;
	number = 0;
	spawnNumber = 100;
	positions.resize(maxNumber);
	velocities.resize(maxNumber);
	lifetime.resize(maxNumber);
	initialSpeedRange = {50.0f,50.0f};
	directionAngleX = 30.0f;
	interval = 0.5f;
	initialLifetime = 3.0f;
	currentInterval = 0.0f;
	loop = true;
	play = true;
	accel = vec3(0.0f, -100.0f, 0.0f);
	size = vec2(1.0f, 1.0f);
	GPUParticleHandle = -1;
	runOnGPU = false;
	lifetimeCounter = 0.0f;
}
