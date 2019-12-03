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
#include "Component/AnimateComponent.h"
#include "Resource/Animator.h"
#include "Singleton/ResourceManager.h"
C_AnimateComponent::C_AnimateComponent():offset(0.0f), scale(1.0f), isPaused(false), animeSpeed(0.0f),isLoop(false),frameSize(0),
					destroyAfterAnime(false), animeFrame(0.0f), pNowPlay(nullptr), defaultPlay(""){

}
C_AnimateComponent::~C_AnimateComponent() {

}
void C_AnimateComponent::Serialize(S_JsonValue &value) {
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "offset") offset = value[itr].asVec2();
		if (itr == "scale") scale = value[itr].asVec2();
		if (itr == "animator")
		{
			std::vector<std::string> animators = value[itr].GetMemberNames();
			for (std::string &ani : animators)
			{
				mAnimators[ani] = C_ResourceManager::GetInstance()->GetAnimator(ani.c_str());
				//mAnimators[ani] = LoadAnimator(ani);
			}
		}
		if (itr == "setNext")
		{
			std::vector<std::string> nexts = value[itr].GetMemberNames();
			for (std::string &next : nexts)
			{
				nextAnimator[next] = value[itr][next].asString();
			}
		}
		if (itr == "defaultPlay") defaultPlay = value[itr].asString();
	}
	InsertAnimator(defaultPlay);
}

void C_AnimateComponent::Initialize() {
	offset = vec2(0.0f);
	scale =  vec2(1.0f);
	isPaused = false;
	animeSpeed = 0.0f;
	isLoop = false;
	frameSize = 0;
	destroyAfterAnime = false;
	animeFrame = 0.0f;
	pNowPlay = nullptr;
	defaultPlay = "";
	mAnimators.clear();
	nextAnimator.clear();
}


void C_AnimateComponent::InsertAnimator(std::string animatorName)
{
	//debug_func::Report("Play "+ animatorName);
	pNowPlay = mAnimators[animatorName];
	if (nullptr != pNowPlay)
	{
		curAniName = animatorName;
		//offset = pNowPlay->mKeyFrames[0].offset;
		//scale = pNowPlay->mKeyFrames[0].scale;
		animeSpeed = pNowPlay->animeSpeed;
		isLoop = pNowPlay->isLoop;
		frameSize = pNowPlay->mFrames.size();
		destroyAfterAnime = pNowPlay->destroyAfterAnime;
		animeFrame = 0.0f;
	}
}