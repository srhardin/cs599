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
#include "System/ControllerSys.h"
#include "Entity.h"
#include "Singleton/InputManager.h"
#include "Config.h"
C_ControllerSystem::C_ControllerSystem() {

}
C_ControllerSystem::~C_ControllerSystem() {

}

void C_ControllerSystem::Register(C_World &worldname, std::vector<int> &entityList)
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if ((worldname.mEntities[entityList[i]].entityMask & std::bitset<64>(E_CPM_PLAYER)).any())
		{
			mPlayerList.insert(worldname.mEntities[entityList[i]].mComponents[E_CP_PLAYER]);
		}
	}
}
void C_ControllerSystem::Unregister(C_World &worldname, std::vector<int> &entityList)
{
	for (int i = 0; i < entityList.size(); ++i)
	{
		if ((worldname.mEntities[entityList[i]].entityMask & std::bitset<64>(E_CPM_PLAYER)).any())
		{
			mPlayerList.erase(worldname.mEntities[entityList[i]].mComponents[E_CP_PLAYER]);
		}
	}
}

void C_ControllerSystem::Update(C_World &worldname, float deltaTime)
{
	for (auto p : mPlayerList)
	{
		unsigned short ID = worldname.mPlayers[p].playerID - 1;
		__int64& cmd = worldname.mPlayers[p].commandList;
		if (ID > C_Config::GetInstance()->key_configs.size() - 1) continue;
		
		unsigned short gamePadNo = C_Config::GetInstance()->key_configs[ID].gamePadNo;
		for (auto keys : C_Config::GetInstance()->key_configs[ID].mKeys)
		{
			switch (keys.state)
			{
				case E_KEY_PRESSED:
				{
					if (C_InputManager::GetInstance()->IsPressed(keys.keyValue, gamePadNo))
					{
						// send command
						cmd = cmd | (__int64(1) << keys.command);
					}
					break;
				}
				case E_KEY_TRIGGERED:
				{
					if (C_InputManager::GetInstance()->IsTriggered(keys.keyValue, gamePadNo))
					{
						// send command
						cmd = cmd | (__int64(1) << keys.command);
					}
					break;
				}
				case E_KEY_RELEASED:
				{
					if (C_InputManager::GetInstance()->IsReleased(keys.keyValue, gamePadNo))
					{
						// send command
						cmd = cmd | (__int64(1) << keys.command);
					}
					break;
				}
				case E_PAD_LEFT_THUMB:
				{
					worldname.mPlayers[p].moveMagnitude = C_InputManager::GetInstance()->LeftThumbMove(worldname.mPlayers[p].moveDirection, gamePadNo);
					if(worldname.mPlayers[p].moveMagnitude > 0)
						cmd = cmd | (__int64(1) << E_PC_MOVE);
					break;
				}
				case E_PAD_RIGHT_THUMB:
				{
					worldname.mPlayers[p].turnMagnitude = C_InputManager::GetInstance()->RightThumbMove(worldname.mPlayers[p].orientation, gamePadNo);
					break;
				}
				case E_PAD_LEFT_TRIGGER:
				{
					if (C_InputManager::GetInstance()->LeftTrigger(gamePadNo))
					{
						// send command
						cmd = cmd | (__int64(1) << keys.command);
					}
					break;
				}
				case E_PAD_RIGHT_TRIGGER:
				{
					if (C_InputManager::GetInstance()->RightTrigger(gamePadNo))
					{
						// send command
						cmd = cmd | (__int64(1) << keys.command);
					}
					break;
				}
			}
		}
	}
}