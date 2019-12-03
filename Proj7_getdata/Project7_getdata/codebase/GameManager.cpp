#include "pch.h"
#include "GameManager.h"
#include "GameStateManager.h"
#include "Singleton/InputManager.h"
#include "Singleton/Utility.h"
#include "System/FrameRateController.h"
#include "Singleton/ResourceManager.h"
#include "System/SystemManager.h"
#include "System/AudioListener.h"
#include "System/CollisionSys.h"
#include "System/Renderer.h"
#include "Tools/Window.h"
#include "Config.h"
C_World *myGameWorld;
C_GameManager::C_GameManager(C_GameStateManager *gsm) {
	// get game state manager
	pGameStateManager = gsm;
	// new all the managers
	pFRC = new C_FrameRateController(60);
	pRenderer = new C_Renderer();
	//pSysManager = new C_SystemManager();
	isPaused = false;
	isSlowed = false;
}

C_GameManager::~C_GameManager() {
	//delete pSysManager;
	delete pRenderer;
	delete pFRC;
	pWindow->releaseWindow();
	delete pWindow;
}

void C_GameManager::GM_GameStart()
{
	C_Config::GetInstance()->LoadConfigs();
	pWindow = new C_Window("Merge", C_Config::GetInstance()->isFullScreen, 1920, 1080);
	pRenderer->RegisterWindow(*pWindow);
	pRenderer->Init();
	C_ResourceManager::GetInstance()->LoadMeshes();
	levelName = "MainMenu";
}

void C_GameManager::GM_Load() {
	pSysManager = new C_SystemManager();
	myGameWorld = new C_World(this);
	C_Utility::GetInstance()->LoadWorld(*myGameWorld, levelName);
	pSysManager->RegisterGOs(*myGameWorld);
}

void C_GameManager::GM_Init() {
	C_Utility::GetInstance()->InitWorld(*myGameWorld);
	InitAttach(*myGameWorld);
	pSysManager->RegisterGOs(*myGameWorld);
	isPaused = false;
}

void C_GameManager::GM_Update() {
	if(isPaused)
		pSysManager->UpdateAll(*myGameWorld, 0);
	else if(isSlowed)
		pSysManager->UpdateAll(*myGameWorld, pFRC->GetFrameTime()/10.0f);
	else
		pSysManager->UpdateAll(*myGameWorld, pFRC->GetFrameTime());
}

void C_GameManager::GM_Draw() {
	if (isPaused)
		pRenderer->Update(*myGameWorld, 0);
	else if (isSlowed)
		pRenderer->Update(*myGameWorld, pFRC->GetFrameTime() / 10.0f);
	else
		pRenderer->Update(*myGameWorld, pFRC->GetFrameTime());
}

void C_GameManager::GM_Free(){
	C_Utility::GetInstance()->FreeWorld(*myGameWorld);

	pSysManager->UnregisterGOs(*myGameWorld);
}

void C_GameManager::GM_Unload() {
	C_Utility::GetInstance()->UnloadWorld(*myGameWorld);
	pSysManager->UnregisterGOs(*myGameWorld);
	delete myGameWorld;
	delete pSysManager;
}

void C_GameManager::GM_FrameStart() {
	pFRC->FrameStart();
}

void C_GameManager::GM_InputUpdate() {
	C_InputManager::GetInstance()->Update();
}

void C_GameManager::GM_FrameEnd() {

	pSysManager->RegisterGOs(*myGameWorld);
	if (myGameWorld->newDeledGOs.size() != 0)
	{
		pSysManager->UnregisterGOs(*myGameWorld);
	}
	pFRC->FrameEnd();

	// check if forcing the application to quit
	pWindow->UpdateWindow();
	if (!pWindow->ShouldRun())
		pGameStateManager->gameStateNext = E_GS_QUIT;

	if (!pWindow->Activated)
	{
		Sleep(250);
	}
}