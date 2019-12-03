/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Ge Daxiao>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "System/SystemManager.h"
#include "System/PhysicsSys.h"
#include "System/ControllerSys.h"
#include "System/AudioListener.h"
#include "System/AnimationSys.h"
#include "System/CollisionSys.h"
#include "System/ParticleSys.h"
#include "System/LuaScriptSys.h"
#include"Entity.h"
#include "Enums.h"
#include "Singleton/Utility.h"
#include "System/Renderer.h"

void InitAttach(C_World& world) {
	for (int i = 0; i < world.mEntities.size(); ++i)
	{
		if ( world.mEntities[i].entityMask==0)
		{
			return;
		}
	
			Attach(world, i, i);
		for (auto j = world.mTransforms[i].childrenID.begin(); j != world.mTransforms[i].childrenID.end();++j)
		{
			Attach(world, i, *j);
		}

	}

}

const std::vector<std::string> C_SystemManager::systemList =
{
	"control",
	"physics",
	"particle",
	"audio",
	"animation",
	"script",
	"collider"
};

void C_SystemManager::DisableComponent(C_World &worldname, std::string sysName, std::vector<int> &delEntityList)
{
	unsigned short sysEnum = C_Utility::GetInstance()->ConvertStringToEnum(sysName, systemList);
	if (sysEnum != E_COLLIDER_SYS)
	{
		mSystems[sysEnum]->Unregister(worldname, delEntityList);
		return;
	}
	else
	{
		mSingletonSystems[0]->Unregister(worldname, delEntityList);
	}
}

void C_SystemManager::EnableComponent(C_World &worldname, std::string sysName, std::vector<int> &EntityList)
{
	unsigned short sysEnum = C_Utility::GetInstance()->ConvertStringToEnum(sysName, systemList);
	if (sysEnum != E_COLLIDER_SYS)
	{
		mSystems[sysEnum]->Register(worldname, EntityList);
		return;
	}
	else if (sysEnum == E_COLLIDER_SYS)
	{
		mSingletonSystems[0]->Register(worldname, EntityList);
	}
}

C_SystemManager::C_SystemManager()
{
	mSystems.push_back(new C_ControllerSystem());
	C_CollisionSystem *pColli = new C_CollisionSystem();
	mSystems.push_back(pColli);
	mSystems.push_back(new C_PhysicsSystem(pColli));
	mSystems.push_back(new C_ParticleSys());
	mSystems.push_back(new C_LuaScriptSys());
	mSystems.push_back(new C_AudioListener());
	mSystems.push_back(new C_AnimationSystem());
	mSingletonSystems.push_back(C_Renderer::globalRenderer);
}
C_SystemManager::~C_SystemManager()
{
	for (auto pSys : mSystems)
	{
		delete pSys;
	}
	mSystems.clear();
}

void C_SystemManager::UpdateAll(C_World &worldname, float deltaTime)
{
	for (auto sys : mSystems)
	{
		sys->Update(worldname,deltaTime);
	}
}
void C_SystemManager::UnregisterGOs(C_World &worldname)
{
	std::vector<int> &delEntityList = worldname.newDeledGOs;
	for (auto sys : mSystems)
	{
		sys->Unregister(worldname, delEntityList);
	}

	for (auto sys : mSingletonSystems)
	{
		sys->Unregister(worldname, delEntityList);
	}

	worldname.DestroyAfterUnregister();
	worldname.newDeledGOs.clear();
}

void C_SystemManager::RegisterGOs(C_World &worldname)
{
	worldname.inRegister = true;
	while (!worldname.newAddedoutRegister->empty()) {
		for (auto sys : mSystems)
		{
			sys->Register(worldname, *worldname.newAddedoutRegister);
		}
		for (auto sys : mSingletonSystems)
		{
			sys->Register(worldname, *worldname.newAddedoutRegister);
		}

		for (int &id : *worldname.newAddedoutRegister)
		{
			worldname.mEntities[id].registered = true;
		}
		worldname.newAddedoutRegister->clear();
		std::swap(worldname.newAddedoutRegister, worldname.newAddedinRegister);
	}
	worldname.inRegister = false;
}