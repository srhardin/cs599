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
#include "System/AudioListener.h"
#include"Bass.h"
#include"Entity.h"
#include "Singleton/ResourceManager.h"
#include"Resource/AudioResource.h"
#include"Enums.h"


float C_AudioListener::sound_effect_volum = 1.0f;
float C_AudioListener::music_volum = 1.0f;

C_AudioListener::C_AudioListener() :mComponents(){

}
C_AudioListener::~C_AudioListener() {}

void C_AudioListener::Register(C_World &worldname, std::vector<int> &entityList) {

	if (entityList.size() == 0) return;

	for (int i = 0; i < entityList.size(); ++i)
	{
		if ((worldname.mEntities[entityList[i]].entityMask & (std::bitset<64>)E_CPM_AUDIO) == E_CPM_AUDIO)
		{
			mComponents.insert(worldname.mEntities[entityList[i]].mComponents[E_CP_AUDIO]);
			const S_AudioResource curr= C_ResourceManager::GetInstance()->GetAudio(worldname.mAudios[worldname.mEntities[entityList[i]].mComponents[E_CP_AUDIO]].mName.c_str());
			if (curr .type== E_SOUND_EFFET)
			{
				BASS_ChannelSetAttribute(curr.mStreamHandle, BASS_ATTRIB_VOL, sound_effect_volum);
			}
			else if(curr.type == E_MUSIC)
			{
				BASS_ChannelSetAttribute(curr.mStreamHandle, BASS_ATTRIB_VOL, music_volum);
			}
		}
	}
}
void C_AudioListener::Unregister(C_World &worldname, std::vector<int> &entityList) {
	if (entityList.size() == 0) return;

	for (int i = 0; i < entityList.size(); ++i)
	{
		if ((worldname.mEntities[entityList[i]].entityMask & (std::bitset<64>)E_CPM_AUDIO) == E_CPM_AUDIO)
		{
			mComponents.erase(worldname.mEntities[entityList[i]].mComponents[E_CP_AUDIO]);
		}
	}
}


void C_AudioListener::SetSoundEffectVolum(float vol) {
	sound_effect_volum = vol;

};

void C_AudioListener::SetMusicVolum(float vol) {
	music_volum = vol;
};

void C_AudioListener::Update(C_World &worldname, float deltaTime) {

	for (auto e : mComponents)
	{

		unsigned int s = worldname.mAudios[e].mState;
		if (s == E_AS_NONE)
		{
			continue;
		}
		HSTREAM temp = C_ResourceManager::GetInstance()->GetAudio(worldname.mAudios[e].mName.c_str()).mStreamHandle;
		HCHANNEL channel = C_ResourceManager::GetInstance()->GetAudio(worldname.mAudios[e].mName.c_str()).mChannel;

		if ((s& E_AS_VOLUME) != 0)
		{
			BASS_ChannelSetAttribute(temp,BASS_ATTRIB_VOL,worldname.mAudios[e].volume);
			worldname.mAudios[e].mState = worldname.mAudios[e].mState & ~E_AS_VOLUME;
		}


		if ((s & E_AS_ENABLE) != 0)
		{

			BASS_ChannelPlay(temp, TRUE);
			worldname.mAudios[e].mState = E_AS_NONE;
	
		}else if ((s & E_AS_DISABLE) != 0)
		{
			BASS_ChannelStop(temp);
			worldname.mAudios[e].mState = E_AS_NONE;
		}

		//unsigned int aaa = E_AS_ENABLE | E_AS_PLAYING;
		if ((s & E_AS_RESTART) !=0 )
		{
			BASS_ChannelPlay(temp, TRUE);
			worldname.mAudios[e].mState = E_AS_NONE;
		}
		else if ((s & E_AS_PLAY) != 0)
		{
			BASS_ChannelPlay(temp, FALSE);
			worldname.mAudios[e].mState = E_AS_NONE;
		}
		else if ((s & E_AS_PAUSE) != 0)
		{
			BASS_ChannelPause(temp);
			BASS_ChannelPlay(temp, FALSE);
			worldname.mAudios[e].mState = E_AS_NONE;
		}
		else if ((s & E_AS_LOOP) != 0)
		{
			BASS_ChannelPlay(temp, FALSE);
		}

		if ((s & E_AS_DESTORY) != 0)
		{
			BASS_ChannelStop(temp);
			worldname.mAudios[e].mState = E_AS_NONE;

		}

	}
}

void C_AudioListener::SetTotalVolum(float vol) {
	BASS_SetVolume(vol);
}