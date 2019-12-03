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
#include "System/AnimationSys.h"
#include "Component/AnimateComponent.h"
#include "Resource/Animator.h"
#include "Singleton/Utility.h"
#include "Entity.h"
#include "Enums.h"
C_AnimationSystem::C_AnimationSystem(){}
C_AnimationSystem::~C_AnimationSystem(){}

void C_AnimationSystem::Register(C_World &worldname, std::vector<int> &entityList) {
	for (auto i: entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_ANIMATION)).any())
		{
			S_AnimationMember newMember = { worldname.mEntities[i].mComponents[E_CP_ANIMATION],
											worldname.mEntities[i].mComponents[E_CP_TRANSFORM]};
			mAnimeList.insert(newMember);
		}
	}
}
void C_AnimationSystem::Unregister(C_World &worldname, std::vector<int> &entityList) {
	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_ANIMATION)).any())
		{
			S_AnimationMember delMember = { worldname.mEntities[i].mComponents[E_CP_ANIMATION],
											worldname.mEntities[i].mComponents[E_CP_TRANSFORM] };
			mAnimeList.erase(delMember);
		}
	}
}

void C_AnimationSystem::Update(C_World &worldname, float deltaTime) {
	for (auto ani : mAnimeList)
	{
		C_AnimateComponent& rAnimation = worldname.mAnimations[ani.aniCP];
		if (nullptr != rAnimation.pNowPlay && !rAnimation.isPaused)
		{
			if (rAnimation.animeFrame < rAnimation.frameSize)
			{
				int keyframe = int(rAnimation.animeFrame);
				rAnimation.offset = rAnimation.pNowPlay->mFrames[keyframe].offset;
				rAnimation.scale = rAnimation.pNowPlay->mFrames[keyframe].scale;
				if (rAnimation.pNowPlay->mFrames[keyframe].isChangeScale) {
					worldname.mTransforms[ani.transCP].Scale = rAnimation.pNowPlay->mFrames[keyframe].TRscale;
				}
				if (rAnimation.pNowPlay->mFrames[keyframe].isChangePosition) {
					worldname.mTransforms[ani.transCP].LocalPos = rAnimation.pNowPlay->mFrames[keyframe].TRposition;
				}
				if (rAnimation.pNowPlay->mFrames[keyframe].isChangeQuaternion) {
					worldname.mTransforms[ani.transCP].LocalQuaternion = rAnimation.pNowPlay->mFrames[keyframe].TRquaternion;
				}
				
				rAnimation.animeFrame += rAnimation.animeSpeed * deltaTime;
			}
			else {
				if (rAnimation.destroyAfterAnime)
				{
					C_Utility::GetInstance()->DestroyEntity(worldname, rAnimation.ownerID);
					return;
				}
				if (rAnimation.isLoop == true)
					rAnimation.animeFrame = 0;
				else
				{
					if("" != rAnimation.nextAnimator[rAnimation.curAniName])
						rAnimation.InsertAnimator(rAnimation.nextAnimator[rAnimation.curAniName]);
					else
						rAnimation.InsertAnimator(rAnimation.defaultPlay);
				}
			}
		}
	}
}