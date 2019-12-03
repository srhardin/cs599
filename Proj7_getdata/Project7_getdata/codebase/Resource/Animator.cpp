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
#include "Resource/Animator.h"



S_Animator::S_Animator():animeSpeed(-1.0f), isLoop(true), destroyAfterAnime(false){
}

S_Animator::~S_Animator() {
}

S_Animator::S_Animator(S_JsonValue& value): animeSpeed(-1.0f), isLoop(true), destroyAfterAnime(false)
{
	std::vector<std::string> names = value.GetMemberNames();
	std::vector<std::pair<KeyFrame, unsigned>> keyFrames;
	for (auto &str : names)
	{
		if (str == "animeSpeed") animeSpeed = value[str].asFloat();
		if (str == "isLoop") isLoop = value[str].asBool();
		if (str == "keyFrame")
		{
			unsigned keep = 1;
			for (int i = 0; i < value[str].size(); ++i)
			{
				std::vector<std::string> frameInfo = value[str][i].GetMemberNames();
				KeyFrame newKeyFrame{ vec2(0.0f),vec2(1.0f),vec3(1.0f),vec3(0.0f),GetQuater(vec4(0.0f, 0.0f, 1.0f, 0.0f)),false,false,false };
				for (auto &w : frameInfo)
				{
					if (w == "offset") newKeyFrame.offset = value[str][i][w].asVec2();
					if (w == "scale") newKeyFrame.scale = value[str][i][w].asVec2();
					if (w == "TRscale")
					{
						newKeyFrame.isChangeScale = true;
						newKeyFrame.TRscale = value[str][i][w].asVec3();
					}

					if (w == "TRposition")
					{
						newKeyFrame.isChangePosition = true;
						newKeyFrame.TRposition = value[str][i][w].asVec3();
					}
					if (w == "TRrotation")
					{
						newKeyFrame.isChangeQuaternion = true;
						vec4 Rotation = value[str][i][w].asVec4();
						Rotation.w = DegreeToRadius(Rotation.w);
						newKeyFrame.TRquaternion = GetQuater(vec4(normalize(vec3(Rotation.x, Rotation.y, Rotation.z)), Rotation.w));
					}
					if (w == "keep") keep = value[str][i][w].asInt();				
				}
				keyFrames.push_back(std::make_pair(newKeyFrame,keep));
			}
		}
		if (str == "destroyAfterAnime") destroyAfterAnime = value[str].asBool();
	}
	loadKeyFrame(keyFrames);
}

void S_Animator::loadKeyFrame(std::vector<std::pair<KeyFrame, unsigned>> keyFrames)
{	
	KeyFrame FirstKeyFrame = keyFrames[0].first;
	int i = 0;
	for (; i < keyFrames.size()-1; ++i)
	{
		//smooth scale
		vec3 intervalScale = (keyFrames[i + 1].first.TRscale - keyFrames[i].first.TRscale) / float(keyFrames[i].second);
		vec3 intervalPosition = (keyFrames[i + 1].first.TRposition - keyFrames[i].first.TRposition) / float(keyFrames[i].second);
		vec4 startQuaternion = keyFrames[i].first.TRquaternion;
		for (int j = 0; j < keyFrames[i].second; ++j)
		{
			keyFrames[i].first.TRquaternion = Slerp(startQuaternion, keyFrames[i + 1].first.TRquaternion, j / float(keyFrames[i].second));
			mFrames.push_back(keyFrames[i].first);
			keyFrames[i].first.TRscale += intervalScale;
			keyFrames[i].first.TRposition += intervalPosition;
		}
	}
	// push last keyframe
	//if (isLoop)
	//{
	//	// smooth between last and first keyframe
	//	vec3 intervalScale = (FirstKeyFrame.TRscale - keyFrames[i].first.TRscale) / float(keyFrames[i].second);
	//	vec3 intervalPosition = (FirstKeyFrame.TRposition - keyFrames[i].first.TRposition) / float(keyFrames[i].second);
	//	for (int j = 0; j < keyFrames[i].second; ++j)
	//	{
	//		keyFrames[i].first.TRquaternion = Slerp(keyFrames[i].first.TRquaternion, FirstKeyFrame.TRquaternion, j / float(keyFrames[i].second));
	//		mFrames.push_back(keyFrames[i].first);
	//		keyFrames[i].first.TRscale += intervalScale;
	//		keyFrames[i].first.TRposition += intervalPosition;
	//	}
	//}
	//else
	//{
	//	for (int j = 0; j < keyFrames[i].second; ++j)
	//	{
	//		mFrames.push_back(keyFrames[i].first);
	//	}
	//}
	for (int j = 0; j < keyFrames[i].second; ++j)
	{
		mFrames.push_back(keyFrames[i].first);
	}
}