/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Wang Zhongqiu>
-End Header --------------------------------------------------------*/
#include"pch.h"
#include"Component/AudioComponent.h"
#include"Singleton/ResourceManager.h"
#include"Resource/AudioResource.h"
C_AudioComponent::C_AudioComponent():mName(""), mState(E_AS_NONE),audioType(E_SOUND_EFFET) {

}

C_AudioComponent::~C_AudioComponent() {

}

void C_AudioComponent::Serialize(S_JsonValue & val) {
	std::vector<std::string> names = val.GetMemberNames();
	for (int i = 0; i < names.size(); ++i)
	{
		if (names[i] == "name")
		{
			mName = val["name"].asString();
		}
		else if (names[i] == "state")
		{
			mState = val["state"].asInt();
		}
		
	}

	const S_AudioResource& tem =  C_ResourceManager::GetInstance()->GetAudio(mName.c_str());

	audioType = tem.type;
}

void C_AudioComponent::Initialize() {
	mName = "";
	mState = 0;
}


