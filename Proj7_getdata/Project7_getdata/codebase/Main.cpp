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
#include "stdio.h"
#include "GameStateManager.h"

FILE _iob[] = { *stdin, *stdout, *stderr };

extern "C" FILE * __cdecl __iob_func(void)
{
	return _iob;
}

#pragma comment(lib, "legacy_stdio_definitions.lib")

int main(int argc, char *argv[])
{
	// Initialize system(window, fps)
	//...
	srand((unsigned)time(NULL));
	C_GameStateManager *gsm = new C_GameStateManager();
	gsm->GSM_Init(E_GS_MAINMANU);
	gsm->GSM_Mainloop();

	delete gsm;

	return 0;
}