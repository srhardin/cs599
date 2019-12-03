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
#include "Config.h"
#include "Singleton/Utility.h"
#include"Bass.h"
C_Config::C_Config()
{
	isFullScreen = true;
	gamePadNumber = 0;
	startLevel = 1;
	resolution = 1;
	systemVolume = 0;
	SFXVolume = 1.0f;
	MusicVolume = 1.0f;
	loadKeyValues();
}

C_Config::~C_Config() {}

void C_Config::LoadConfigs()
{
	S_JsonValue value = json_func::LoadJsonData("..//..//Resource//Json//Configs.json");
	isFullScreen = value["FullScreen"].asBool();
	if (!isFullScreen)
	{
		resolution = 2;
	}
	gamePadNumber = value["gamePadNumber"].asInt();
	for (int i = 0; i< value["keyConfig"].size();++i)
	{
		S_KEY_CONFIG newConfig;
		std::vector<std::string> keyConfigs = value["keyConfig"][i].GetMemberNames();
		newConfig.gamePadNo = value["keyConfig"][i]["GamePad"].asInt();
		for (auto k : keyConfigs)
		{
			if (k == "GamePad") continue;
			S_KEY_COMMAND newCommand;
			newCommand.command = C_Utility::GetInstance()->ConvertStringToEnum(k, PlayerCommandList);
			newCommand.state = C_Utility::GetInstance()->ConvertStringToEnum(value["keyConfig"][i][k][0].asString(), KeyStateList);
			for (int j =0 ; j < value["keyConfig"][i][k][1].size();++j)
			{
				newCommand.keyValue = KeyValue[value["keyConfig"][i][k][1][j].asString()];
				newConfig.mKeys.push_back(newCommand);
			}
		}
		key_configs.push_back(newConfig);
	}
}

void C_Config::SaveSystemVolume()
{
	systemVolume = BASS_GetVolume();
	gameVolume = sqrtf(systemVolume);
}
void C_Config::ReturnSystemVolume()
{
	BASS_SetVolume(systemVolume);
}

const std::vector<std::string> C_Config::PlayerCommandList =
{
	"move",
	"move_up",
	"move_down",
	"move_left",
	"move_right",
	"rotate_left",
	"rotate_right",
	"hold",
	"aim",
	"skillA",
	"skillB",
	"skillX",
	"skillY",
	"detach1",
	"detach2",
	"ultimate",
	"taunt",
};

const std::vector<std::string> C_Config::KeyStateList= 
{
	"press",
	"trigger",
	"release",
	"left_trigger",
	"right_trigger",
	"left_thumb",
	"right_thumb"
};

void C_Config::loadKeyValues() {
	KeyValue["NULL"] = 0;
	KeyValue["W"] = VK_W;
	KeyValue["A"] = VK_A;
	KeyValue["S"] = VK_S;
	KeyValue["D"] = VK_D;
	KeyValue["SPACE"] = VK_SPACE;
	KeyValue["Q"] = VK_Q;
	KeyValue["E"] = VK_E;
	KeyValue["R"] = VK_R;
	KeyValue["T"] = VK_T;
	KeyValue["Z"] = VK_Z;
	KeyValue["X"] = VK_X;
	KeyValue["C"] = VK_C;
	KeyValue["V"] = VK_V;
	KeyValue["B"] = VK_B;
	KeyValue["N"] = VK_N;
	KeyValue["M"] = VK_M;
	KeyValue["L"] = VK_L;
	KeyValue["K"] = VK_K;
	KeyValue["J"] = VK_J;
	KeyValue["P"] = VK_P;
	KeyValue["O"] = VK_O;
	KeyValue["I"] = VK_I;
	KeyValue["U"] = VK_U;
	KeyValue["F"] = VK_F;
	KeyValue["G"] = VK_G;
	KeyValue["H"] = VK_H;
	KeyValue["Y"] = VK_Y;

	KeyValue["1"] = VK_1;
	KeyValue["2"] = VK_2;
	KeyValue["3"] = VK_3;
	KeyValue["4"] = VK_4;
	KeyValue["5"] = VK_5;
	KeyValue["6"] = VK_6;
	KeyValue["7"] = VK_7;
	KeyValue["8"] = VK_8;
	KeyValue["9"] = VK_9;
	KeyValue["0"] = VK_0;

	KeyValue["F1"] = VK_F1;
	KeyValue["F2"] = VK_F2;
	KeyValue["F3"] = VK_F3;
	KeyValue["F4"] = VK_F4;
	KeyValue["F5"] = VK_F5;
	KeyValue["F6"] = VK_F6;
	KeyValue["F7"] = VK_F7;
	KeyValue["F8"] = VK_F8;
	KeyValue["F9"] = VK_F9;
	KeyValue["F10"] = VK_F10;
	KeyValue["F11"] = VK_F11;
	KeyValue["F12"] = VK_F12;

	KeyValue["UP"] = VK_UP;
	KeyValue["DOWN"] = VK_DOWN;
	KeyValue["LEFT"] = VK_LEFT;
	KeyValue["RIGHT"] = VK_RIGHT;
	KeyValue["SHIFT"] = VK_SHIFT;
	KeyValue["CTRL"] = VK_CONTROL;
	KeyValue["ESC"] = VK_ESCAPE;

	KeyValue["PAD_UP"] = XINPUT_GAMEPAD_DPAD_UP;
	KeyValue["PAD_DOWN"] = XINPUT_GAMEPAD_DPAD_DOWN;
	KeyValue["PAD_LEFT"] = XINPUT_GAMEPAD_DPAD_LEFT;
	KeyValue["PAD_RIGHT"] = XINPUT_GAMEPAD_DPAD_RIGHT;
	KeyValue["PAD_START"] = XINPUT_GAMEPAD_START;
	KeyValue["PAD_BACK"] = XINPUT_GAMEPAD_BACK;
	KeyValue["LEFT_THUMB"] = XINPUT_GAMEPAD_LEFT_THUMB;
	KeyValue["RIGHT_THUMB"] = XINPUT_GAMEPAD_RIGHT_THUMB;
	KeyValue["LEFT_SHOULDER"] = XINPUT_GAMEPAD_LEFT_SHOULDER;
	KeyValue["RIGHT_SHOULDER"] = XINPUT_GAMEPAD_RIGHT_SHOULDER;
	KeyValue["PAD_A"] = XINPUT_GAMEPAD_A;
	KeyValue["PAD_B"] = XINPUT_GAMEPAD_B;
	KeyValue["PAD_X"] = XINPUT_GAMEPAD_X;
	KeyValue["PAD_Y"] = XINPUT_GAMEPAD_Y;
	KeyValue["LEFT_TRIGGER"] = VK_PAD_LTRIGGER;
	KeyValue["RIGHT_TRIGGER"] = VK_PAD_RTRIGGER;
}