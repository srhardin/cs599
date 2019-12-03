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
#include "Entity.h"
#include "Enums.h"
#include "GameManager.h"
#include "System/SystemManager.h"
C_Entity::C_Entity() :name(""), entityMask(E_CPM_NONE), mComponents(E_CP_NUMS, -1),registered(false)
{

}

C_Entity::~C_Entity()
{

}

//C_World::C_World() {}

C_World::C_World(C_GameManager* pGM) : pMyGM(pGM), miu(0.3f),gravity(0.0f), mEntities(MAX_ENTITY_COUNT), mTransforms(MAX_ENTITY_COUNT),
mBodies(MAX_ENTITY_COUNT), mAudios(MAX_ENTITY_COUNT), mMeshs(MAX_ENTITY_COUNT), mMaterials(MAX_ENTITY_COUNT), mCameras(16), mPlayers(16),
mAnimations(MAX_ENTITY_COUNT), mCircleColliders(MAX_ENTITY_COUNT), mPolygonColliders(MAX_ENTITY_COUNT), mParticles(MAX_ENTITY_COUNT), mLuaComponents(MAX_ENTITY_COUNT),
mTagComponents(MAX_ENTITY_COUNT), mWidgetComponents(MAX_ENTITY_COUNT),mTextComponents(MAX_ENTITY_COUNT)
{
	Initialize();
	AddressComponents();
	recycledMark.reserve(512);
	newAddedGOs.reserve(1024);
	newAddedGOsBuffer.reserve(1024);
	newAddedoutRegister = &newAddedGOs;
	newAddedinRegister = &newAddedGOsBuffer;
	newDeledGOs.reserve(1024);
}

C_World::C_World(int entityNO):pMyGM(nullptr), miu(0.3f), gravity(0.0f), mEntities(entityNO), mTransforms(entityNO),
mBodies(entityNO), mAudios(entityNO), mMeshs(entityNO), mMaterials(entityNO), mCameras(16), mPlayers(16),
mAnimations(entityNO), mCircleColliders(entityNO), mPolygonColliders(entityNO), mParticles(entityNO), mLuaComponents(entityNO),
mTagComponents(entityNO), mWidgetComponents(entityNO), mTextComponents(entityNO)
{
	Initialize();
	AddressComponents();
	newAddedoutRegister = &newAddedGOs;
	newAddedinRegister = &newAddedGOsBuffer;
}

C_World::~C_World()
{
}

void C_World::Initialize()
{
	mComponentsLastIndex.resize(E_CP_NUMS);
	for (int i = 0; i < E_CP_NUMS; ++i)
	{
		mDeletedComponents.push_back(std::queue<unsigned>());
	}
	lastEntityIndex = 0;
}

void C_World::AddressComponents()
{
	// register pointers of all components
	{
		std::vector<C_Component*> transforms;
		for (auto &tr : mTransforms)
		{
			transforms.push_back(&tr);
		}
		mComponentsAddresses.push_back(transforms);
	}
	{
		std::vector<C_Component*> bodies;
		for (auto &bd : mBodies)
		{
			bodies.push_back(&bd);
		}
		mComponentsAddresses.push_back(bodies);
	}
	{
		std::vector<C_Component*> audios;
		for (auto &ad : mAudios)
		{
			audios.push_back(&ad);
		}
		mComponentsAddresses.push_back(audios);
	}
	{
		std::vector<C_Component*> meshes;
		for (auto &ms : mMeshs)
		{
			meshes.push_back(&ms);
		}
		mComponentsAddresses.push_back(meshes);
	}
	{
		std::vector<C_Component*> materials;
		for (auto &mt : mMaterials)
		{
			materials.push_back(&mt);
		}
		mComponentsAddresses.push_back(materials);
	}
	{
		std::vector<C_Component*> cameras;
		for (auto &ca : mCameras)
		{
			cameras.push_back(&ca);
		}
		mComponentsAddresses.push_back(cameras);
	}
	{
		std::vector<C_Component*> players;
		for (auto &pl : mPlayers)
		{
			players.push_back(&pl);
		}
		mComponentsAddresses.push_back(players);
	}
	{
		std::vector<C_Component*> animations;
		for (auto &an : mAnimations)
		{
			animations.push_back(&an);
		}
		mComponentsAddresses.push_back(animations);
	}
	{
		std::vector<C_Component*> c_colliders;
		for (auto &cl : mCircleColliders)
		{
			c_colliders.push_back(&cl);
		}
		mComponentsAddresses.push_back(c_colliders);
	}
	{
		std::vector<C_Component*> b_colliders;
		for (auto &bx : mPolygonColliders)
		{
			b_colliders.push_back(&bx);
		}
		mComponentsAddresses.push_back(b_colliders);
	}
	{
		std::vector<C_Component*> particles;
		for (auto &pa : mParticles)
		{
			particles.push_back(&pa);
		}
		mComponentsAddresses.push_back(particles);
	}
	{
		std::vector<C_Component*> luas;
		for (auto &la : mLuaComponents)
		{
			luas.push_back(&la);
		}
		mComponentsAddresses.push_back(luas);
	}
	{
		std::vector<C_Component*> tags;
		for (auto &tg : mTagComponents)
		{
			tags.push_back(&tg);
		}
		mComponentsAddresses.push_back(tags);
	}
	{
		std::vector<C_Component*> widgets;
		for (auto &w : mWidgetComponents)
		{
			widgets.push_back(&w);
		}
		mComponentsAddresses.push_back(widgets);
	}
	{
		std::vector<C_Component*> texts;
		for (auto &w : mTextComponents)
		{
			texts.push_back(&w);
		}
		mComponentsAddresses.push_back(texts);
	}
}

void C_World::resizeComponentList(unsigned cpNo)
{
	switch (cpNo)
	{
	case E_CP_TRANSFORM:
	{
		size_t formerIndex = mTransforms.size();
		mTransforms.resize(formerIndex * 2);
		debug_func::ReportWarning("transform full! resize deque to " + std::to_string(mTransforms.size()) + "!");
		for (; formerIndex < mTransforms.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mTransforms[formerIndex]);
		}
		break;
	}
	case E_CP_BODY:
	{
		size_t formerIndex = mBodies.size();
		mBodies.resize(formerIndex * 2);
		debug_func::ReportWarning("mBodies full! resize deque to " + std::to_string(mBodies.size()) + "!");
		for (; formerIndex < mBodies.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mBodies[formerIndex]);
		}
		break;
	}
	case E_CP_AUDIO:
	{
		size_t formerIndex = mAudios.size();
		mAudios.resize(formerIndex * 2);
		debug_func::ReportWarning("mAudios full! resize deque to " + std::to_string(mAudios.size()) + "!");
		for (; formerIndex < mAudios.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mAudios[formerIndex]);
		}
		break;
	}
	case E_CP_MESH:
	{
		size_t formerIndex = mMeshs.size();
		mMeshs.resize(formerIndex * 2);
		debug_func::ReportWarning("mMeshs full! resize deque to " + std::to_string(mMeshs.size()) + "!");
		for (; formerIndex < mMeshs.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mMeshs[formerIndex]);
		}
		break;
	}
	case E_CP_MATERIAL:
	{
		size_t formerIndex = mMaterials.size();
		mMaterials.resize(formerIndex * 2);
		debug_func::ReportWarning("mMaterials full! resize deque to " + std::to_string(mMaterials.size()) + "!");
		for (; formerIndex < mMaterials.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mMaterials[formerIndex]);
		}
		break;
	}
	case E_CP_CAMERA:
	{
		size_t formerIndex = mCameras.size();
		mCameras.resize(formerIndex * 2);
		debug_func::ReportWarning("mCameras full! resize deque to " + std::to_string(mCameras.size()) + "!");
		for (; formerIndex < mCameras.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mCameras[formerIndex]);
		}
		break;
	}
	case E_CP_PLAYER:
	{
		size_t formerIndex = mPlayers.size();
		mPlayers.resize(formerIndex * 2);
		debug_func::ReportWarning("mPlayers full! resize deque to " + std::to_string(mPlayers.size()) + "!");
		for (; formerIndex < mPlayers.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mPlayers[formerIndex]);
		}
		break;
	}
	case E_CP_ANIMATION:
	{
		size_t formerIndex = mAnimations.size();
		mAnimations.resize(formerIndex * 2);
		debug_func::ReportWarning("mAnimations full! resize deque to " + std::to_string(mAnimations.size()) + "!");
		for (; formerIndex < mAnimations.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mAnimations[formerIndex]);
		}
		break;
	}
	case E_CP_CIRCLE_COLLIDER:
	{
		size_t formerIndex = mCircleColliders.size();
		mCircleColliders.resize(formerIndex * 2);
		debug_func::ReportWarning("mCircleColliders full! resize deque to " + std::to_string(mCircleColliders.size()) + "!");
		for (; formerIndex < mCircleColliders.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mCircleColliders[formerIndex]);
		}
		break;
	}
	case E_CP_BOX_COLLIDER:
	{
		size_t formerIndex = mPolygonColliders.size();
		mPolygonColliders.resize(formerIndex * 2);
		debug_func::ReportWarning("mPolygonColliders full! resize deque to " + std::to_string(mPolygonColliders.size()) + "!");
		for (; formerIndex < mPolygonColliders.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mPolygonColliders[formerIndex]);
		}
		break;
	}
	case E_CP_PARTICLE:
	{
		size_t formerIndex = mParticles.size();
		mParticles.resize(formerIndex * 2);
		debug_func::ReportWarning("mParticles full! resize deque to " + std::to_string(mParticles.size()) + "!");
		for (; formerIndex < mParticles.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mParticles[formerIndex]);
		}
		break;
	}
	case E_CP_SCRIPT:
	{
		size_t formerIndex = mLuaComponents.size();
		mLuaComponents.resize(formerIndex * 2);
		debug_func::ReportWarning("mLuaComponents full! resize deque to " + std::to_string(mLuaComponents.size()) + "!");
		for (; formerIndex < mLuaComponents.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mLuaComponents[formerIndex]);
		}
		break;
	}
	case E_CP_TAG:
	{
		size_t formerIndex = mTagComponents.size();
		mTagComponents.resize(formerIndex * 2);
		debug_func::ReportWarning("mTagComponents full! resize deque to " + std::to_string(mTagComponents.size()) + "!");
		for (; formerIndex < mTagComponents.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mTagComponents[formerIndex]);
		}
		break;
	}
	case E_CP_WIDGET:
	{
		size_t formerIndex = mWidgetComponents.size();
		mWidgetComponents.resize(formerIndex * 2);
		debug_func::ReportWarning("mWidgetComponents full! resize deque to " + std::to_string(mWidgetComponents.size()) + "!");
		for (; formerIndex < mWidgetComponents.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mWidgetComponents[formerIndex]);
		}
		break;
	}
	case E_CP_TEXT:
	{
		size_t formerIndex = mTextComponents.size();
		mTextComponents.resize(formerIndex * 2);
		debug_func::ReportWarning("mTextComponents full! resize deque to " + std::to_string(mTextComponents.size()) + "!");
		for (; formerIndex < mTextComponents.size(); ++formerIndex)
		{
			mComponentsAddresses[cpNo].push_back(&mTextComponents[formerIndex]);
		}
		break;
	}
	}
}

void C_World::DestroyAfterUnregister()
{
	for (int ei = 0; ei < newDeledGOs.size(); ++ei) {
		unsigned del = newDeledGOs[ei];
		C_Entity &t = mEntities[del];
		if (t.entityMask == E_CPM_NONE) continue;
		t.inDelList = false;
		t.registered = false;
		// earse it in game scene
		nameToID[t.name].erase(del);
		// destroy or recycle
		if (recycledMark[ei] == false) {
			debug_func::Report("push into deletedList, name: "+ mEntities[del].name+" id: " + std::to_string(del));
			deletedList.push(del);
			for (unsigned i = 0; i < E_CP_NUMS; ++i)
			{
				unsigned& cpID = t.mComponents[i];
				if (cpID != -1)
				{
					// set component's owner to -1
					mComponentsAddresses[i][cpID]->ownerID = -1;
					mComponentsAddresses[i][cpID]->Initialize();
					mDeletedComponents[i].push(cpID);
					cpID = -1;
				}
			}
			debug_func::Report("destroy " + t.name + "id: " + std::to_string(del));
			t.entityMask = E_CPM_NONE;
			t.name = "";
		}
		else
		{
			debug_func::Report("put into recycle " + mEntities[del].name);
			recycledList.push_back(del);
		}
	}
	recycledMark.clear();
}

void C_World::PushNewEntity(unsigned entityID)
{
	if (inRegister)
	{
		newAddedinRegister->push_back(entityID);
	}
	else
	{
		newAddedoutRegister->push_back(entityID);
	}
}

 bool C_World::HasComponent(unsigned int entity, E_COMPONENTS_MASK mask) {
	
	 return (mEntities[entity].entityMask & std::bitset<64>(mask)).any();
}
 unsigned C_World::GetComponent(unsigned int entity, E_COMPONENTS_NAME component) {	
	 return mEntities[entity].mComponents[component];
 }


 bool HasComponent(C_World& worldName, unsigned int entity, E_COMPONENTS_MASK mask) {

	 return (worldName.mEntities[entity].entityMask & std::bitset<64>(mask)).any();
 };

 unsigned GetComponent(C_World& worldName, unsigned int entity, E_COMPONENTS_NAME component) {

	 return worldName.mEntities[entity].mComponents[component];
 }
