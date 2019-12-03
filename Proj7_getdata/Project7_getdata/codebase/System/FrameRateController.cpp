/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Li Chunyan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "System/FrameRateController.h"

/*Constructor
* Parameter:
*		MaxFramerate:frame rate
*/
C_FrameRateController::C_FrameRateController(int MaxFramerate)
{
	this->mTickStart = this->mTickEnd = this->mframeTime = 0;
	if (0 != MaxFramerate)
		mNeededTicksPerFrame = 1000 / MaxFramerate;
	else
		mNeededTicksPerFrame = 0;
}

C_FrameRateController::~C_FrameRateController()
{
}

/*Get start Tick time
* Return tick time mTickStart
*/
void C_FrameRateController::FrameStart()
{
	this->mTickStart = clock();
}

/*Get end Tick time
* Parameters:
*		mframeTime: frame time
*       mTickEnd:frame end tick time
*/
void C_FrameRateController::FrameEnd()
{
	this->mTickEnd = clock();

	while ((mTickEnd - mTickStart) < mNeededTicksPerFrame)
	{
		mTickEnd = clock();
	}
	this->mframeTime = (mTickEnd - mTickStart);
}

/*Return tick time mTickStart*/
float C_FrameRateController::GetFrameTime()
{
	float deltaTime = this->mframeTime / 1000.0;
	
	deltaTime = deltaTime < 0.04f ? deltaTime : 0.03f;
	
	return deltaTime;
}

int C_FrameRateController::GetFrame()
{
	float frameTime = GetFrameTime();

	return (int)1.0/frameTime;
}
