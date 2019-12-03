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
#include "GameStateManager.h"
#include "GameManager.h"
#include "Config.h"
C_GameManager *gpGM;

C_GameStateManager::C_GameStateManager() {
	gpGM = new C_GameManager(this);
}
C_GameStateManager::~C_GameStateManager() {
	delete gpGM;
}

void C_GameStateManager::GSM_Init(unsigned int _gameStateInit) {
	gameStateInit = _gameStateInit;

	gameStateCurr =	gameStatePrev =	gameStateNext = gameStateInit;

	gpGM->GM_GameStart();
}

void C_GameStateManager::GSM_Mainloop() {
	while (gameStateCurr != E_GS_QUIT)
	{
		
		if (gameStateCurr == E_GS_RESTART) 
			gameStateNext = gameStateCurr = gameStatePrev;
		else
			gpGM->GM_Load();

		// Initialize the gamestate
		gpGM->GM_Init();

		while (gameStateCurr == gameStateNext)
		{
			gpGM->GM_FrameStart();

			gpGM->GM_InputUpdate();

			gpGM->GM_Update();

			gpGM->GM_Draw();

			gpGM->GM_FrameEnd();
		}

		gpGM->GM_Free();

		if (gameStateNext != E_GS_RESTART)
			gpGM->GM_Unload();

		gameStatePrev = gameStateCurr;
		gameStateCurr = gameStateNext;
	}
	// return system volunm
	C_Config::GetInstance()->ReturnSystemVolume();
}