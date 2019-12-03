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
#include"Resource/AudioResource.h"
#include "Enums.h"
S_AudioResource::S_AudioResource(const void *file) {
	mStreamHandle = BASS_StreamCreateFile(FALSE, file, 0, 0, BASS_SAMPLE_FLOAT | !BASS_SAMPLE_LOOP);
	//mStreamHandle = BASS_StreamCreateFile(FALSE, "F:\\darkflare\\Resource\\Music\\Numb.mp3", 0, 0, BASS_SAMPLE_FLOAT);
	if (mStreamHandle == 0)
	{
		printf("load audio %s failed.\n", file);
		return;
	}
	mChannel = BASS_SampleGetChannel(mStreamHandle,FALSE);
	
	HMUSIC aa = BASS_MusicLoad(FALSE, file,0,0, BASS_SAMPLE_FLOAT | BASS_SAMPLE_LOOP,14400);
	int a = BASS_ErrorGetCode();
	BASS_ChannelSetAttribute(aa,BASS_ATTRIB_MUSIC_SPEED,10.0f);
	BASS_ChannelPlay(aa, TRUE);
	type = E_SOUND_EFFET;
}

S_AudioResource::S_AudioResource() {
	mStreamHandle = 0;
	mChannel = 0;
}


S_AudioResource::~S_AudioResource() {
	BASS_SampleFree(mStreamHandle);
}