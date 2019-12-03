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
#include "Singleton/InputManager.h"
#include "Config.h"
#include "System/Renderer.h"
//#include "Tools/LuaHelper.h"
#pragma comment(lib, "XInput.lib") 
C_InputManager::C_InputManager() {
	memset(mCurrentState,0,256*sizeof(char));
	ZeroMemory(xCurrentState, 4*sizeof(XINPUT_STATE));
}

C_InputManager::~C_InputManager() {

}

void C_InputManager::Update() {
	if (C_Renderer::globalRenderer->isWindowActivated()) {
		memcpy(mPreviousState, mCurrentState, 256 * sizeof(char));
		memcpy(xPreviousState, xCurrentState, 4 * sizeof(XINPUT_STATE));
		memcpy(xPreviousLeftTrigger, xCurrentLeftTrigger, 4 * sizeof(bool));
		memcpy(xPreviousRightTrigger, xCurrentRightTrigger, 4 * sizeof(bool));

		for (int i = 0; i < 256; ++i) {
			mCurrentState[i] = (char)(GetAsyncKeyState(i) >> 8);
		}

		// game pad
		for (int i = 0; i < C_Config::GetInstance()->gamePadNumber; ++i)
		{
			ZeroMemory(&xCurrentState[i], sizeof(XINPUT_STATE));

			// Simply get the state of the controller from XInput.
			dwResult[i] = XInputGetState(i, &xCurrentState[i]);
			// transform trigger to key value
			xCurrentLeftTrigger[i] = LeftTrigger(i) ? true : false;
			xCurrentRightTrigger[i] = RightTrigger(i) ? true : false;
		}
	}
}

bool C_InputManager::PadConnected(int num)
{
	if (num > 4 or num < 1) return false;
	if (dwResult[num - 1] == 0)
		return true;
	return false;
}

bool C_InputManager::IsPressed(uint16_t _keyScanCode, uint16_t padID) {
	// gamepad
	if ((xCurrentState[padID].Gamepad.wButtons & _keyScanCode) == _keyScanCode)
		return true;
	if (_keyScanCode == VK_PAD_LTRIGGER)
		return xCurrentLeftTrigger[padID];
	if (_keyScanCode == VK_PAD_RTRIGGER)
		return xCurrentRightTrigger[padID];
	// keyboard
	if (_keyScanCode >= 256)
		return false;

	if (mCurrentState[_keyScanCode])
		return true;

	return false;
}

bool C_InputManager::IsTriggered(uint16_t _keyScanCode, uint16_t padID) {
	// gamepad
	if ((xCurrentState[padID].Gamepad.wButtons & _keyScanCode) == _keyScanCode && !(xPreviousState[padID].Gamepad.wButtons & _keyScanCode))
		return true;
	if (_keyScanCode == VK_PAD_LTRIGGER)
		return (xCurrentLeftTrigger[padID] && !xPreviousLeftTrigger[padID]);
	if (_keyScanCode == VK_PAD_RTRIGGER)
		return (xCurrentRightTrigger[padID] && !xPreviousRightTrigger[padID]);
	// keyboard
	if (_keyScanCode >= 256)
		return false;

	if (mCurrentState[_keyScanCode] && !mPreviousState[_keyScanCode])
		return true;

	return false;
}

bool C_InputManager::IsReleased(uint16_t _keyScanCode, uint16_t padID) {
	// gamepad
	if (!(xCurrentState[padID].Gamepad.wButtons & _keyScanCode) && (xPreviousState[padID].Gamepad.wButtons & _keyScanCode) == _keyScanCode)
		return true;
	if (_keyScanCode == VK_PAD_LTRIGGER)
		return (!xCurrentLeftTrigger[padID] && xPreviousLeftTrigger[padID]);
	if (_keyScanCode == VK_PAD_RTRIGGER)
		return (!xCurrentRightTrigger[padID] && xPreviousRightTrigger[padID]);
	// keyboard
	if (_keyScanCode >= 256)
		return false;

	if (!mCurrentState[_keyScanCode] && mPreviousState[_keyScanCode])
		return true;

	return false;
}


float C_InputManager::LeftThumbMove(vec2 &dir, uint16_t padID)
{
	// no gamepad connectted or no config
	if (dwResult[padID] != ERROR_SUCCESS)
		return 0.0f;
	float LX = xCurrentState[padID].Gamepad.sThumbLX;
	float LY = xCurrentState[padID].Gamepad.sThumbLY;

	//determine how far the controller is pushed
	float sqrMagnitude = LX*LX + LY * LY;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (sqrMagnitude > XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE*XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE)
	{
		float magnitude = sqrt(sqrMagnitude);
		//determine the direction the controller is pushed
		dir.x = LX / magnitude;
		dir.y = LY / magnitude;
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		//magnitude = 0.0;
		dir = vec2(0.0f,0.0f);
		normalizedMagnitude = 0.0;
	}

	return normalizedMagnitude;
}

float C_InputManager::RightThumbMove(vec2 &dir, uint16_t padID)
{
	// no gamepad connectted or no config
	if (dwResult[padID] != ERROR_SUCCESS)
		return 0.0f;
	float RX = xCurrentState[padID].Gamepad.sThumbRX;
	float RY = xCurrentState[padID].Gamepad.sThumbRY;

	//determine how far the controller is pushed
	float sqrMagnitude = RX * RX + RY * RY;

	float normalizedMagnitude = 0;

	//check if the controller is outside a circular dead zone
	if (sqrMagnitude > XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE*XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE)
	{
		float magnitude = sqrt(sqrMagnitude);
		//determine the direction the controller is pushed
		dir.x = RX / magnitude;
		dir.y = RY / magnitude;
		//clip the magnitude at its expected maximum value
		if (magnitude > 32767) magnitude = 32767;

		//adjust magnitude relative to the end of the dead zone
		magnitude -= XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE;

		//optionally normalize the magnitude with respect to its expected range
		//giving a magnitude value of 0.0 to 1.0
		normalizedMagnitude = magnitude / (32767 - XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE);
	}
	else //if the controller is in the deadzone zero out the magnitude
	{
		//magnitude = 0.0;
		dir = vec2(0.0f, 0.0f);
		normalizedMagnitude = 0.0;
	}

	return normalizedMagnitude;
}

bool C_InputManager::LeftTrigger(uint16_t padID)
{
	if (xCurrentState[padID].Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;
	return false;
}

bool C_InputManager::RightTrigger(uint16_t padID)
{
	if (xCurrentState[padID].Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		return true;
	return false;
}

bool C_InputManager::AnyKeyPressed()
{
	for (int i = 0; i < C_Config::GetInstance()->gamePadNumber; ++i)
	{
		if (xCurrentState[i].Gamepad.wButtons != 0) 
			return true;
	}
	for (int i = 0; i < 256; ++i) {
		if (mCurrentState[i] != 0)
			return true;
	}
	return false;
}