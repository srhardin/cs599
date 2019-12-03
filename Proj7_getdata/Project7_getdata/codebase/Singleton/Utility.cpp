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
#include "Singleton/Utility.h"
#include "Singleton/ResourceManager.h"
#include "Entity.h"
#include "Enums.h"
#include "GameManager.h"
extern C_GameManager *gpGM;
const std::vector<std::string> C_Utility::ComponentList =
{
	"transform",
	"body",
	"audio",
	"mesh",
	"material",
	"camera",
	"player",
	"animation",
	"circleCollider",
	"polygonCollider",
	"particle",
	"script",
	"tag",
	"widget",
	"text"
};

const std::string& C_Utility::ConvertEnumToString(unsigned cpName) const{
	return C_Utility::ComponentList[cpName];
}

unsigned C_Utility::ConvertStringToEnum(const std::string &pStr, const std::vector<std::string>& list) const{
	unsigned Enum = -1;
	for (int i = 0; i < list.size(); ++i)
	{
		if (pStr == list[i])
		{
			Enum = i;
			break;
		}
	}
	return Enum;
}

unsigned C_Utility::CreateEntity(C_World &worldName, const std::string &_name)
{
	unsigned entity = 0;
	if (worldName.deletedList.empty()) {
		if (worldName.lastEntityIndex >= worldName.mEntities.size())
		{
			worldName.mEntities.resize(worldName.mEntities.size() * 2);
			debug_func::ReportErr("Entity full! resize deque to " + std::to_string(worldName.mEntities.size()) + "!");
		}
		entity = worldName.lastEntityIndex++;
		debug_func::Report("Get from lastEntityIndex: " + std::to_string(entity));
	}
	else
	{
		entity = worldName.deletedList.front();
		worldName.deletedList.pop();
		debug_func::Report("Get from deletedList: " + std::to_string(entity));
	}
	worldName.mEntities[entity].entityMask = E_CPM_NONE;	//clear mask
	//worldName.mEntities[entity].entityMask = worldName.mEntities[entity].entityMask | std::bitset<64>(E_GO_BE_USING) << 32; // set used
	worldName.mEntities[entity].name = _name;
	worldName.nameToID[_name].insert(entity);
	worldName.PushNewEntity(entity);
	worldName.moduleObjects[nowmodules].push_back(entity);
	debug_func::Report("CreateEntity "+ _name+ "id: "+ std::to_string(entity));
	return entity;
}

void C_Utility::CreatePrefabs(const std::string &_name)
{
	debug_func::Report("Create prefab!");
	prefabName[_name] = C_ResourceManager::GetInstance()->LoadGameObjects(*prefabsWorld, _name.c_str());
}

unsigned C_Utility::CopyGameObject(C_World &worldName, const unsigned copyentityID)
{
	int entityID = -1;

	entityID = CreateEntity(worldName, prefabsWorld->mEntities[copyentityID].name);
	// copy all components
	std::vector<unsigned> &prefabCPs = prefabsWorld->mEntities[copyentityID].mComponents;
	if (prefabCPs[E_CP_TRANSFORM] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_TRANSFORM);
		worldName.mTransforms[cpID] = prefabsWorld->mTransforms[prefabCPs[E_CP_TRANSFORM]];
		worldName.mTransforms[cpID].ownerID = entityID;
		std::set<unsigned> &prefabChildrenID = prefabsWorld->mTransforms[prefabCPs[E_CP_TRANSFORM]].childrenID;
		if (!prefabChildrenID.empty())
		{
			worldName.mTransforms[cpID].childrenID.clear();
			// create children and set parent relationship
			for (auto child : prefabChildrenID)
			{
				unsigned newChildID = CopyGameObject(worldName, prefabsWorld->mTransforms[child].ownerID);
				unsigned newChildTRID = worldName.mEntities[newChildID].mComponents[E_CP_TRANSFORM];
				worldName.mTransforms[newChildTRID].parentID = cpID;
				worldName.mTransforms[cpID].childrenID.insert(newChildTRID);
			}
		}
	}
	if (prefabCPs[E_CP_BODY] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_BODY);
		worldName.mBodies[cpID] = prefabsWorld->mBodies[prefabCPs[E_CP_BODY]];
		worldName.mBodies[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_AUDIO] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_AUDIO);
		worldName.mAudios[cpID] = prefabsWorld->mAudios[prefabCPs[E_CP_AUDIO]];
		worldName.mAudios[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_MESH] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_MESH);
		worldName.mMeshs[cpID] = prefabsWorld->mMeshs[prefabCPs[E_CP_MESH]];
		worldName.mMeshs[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_MATERIAL] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_MATERIAL);
		worldName.mMaterials[cpID] = prefabsWorld->mMaterials[prefabCPs[E_CP_MATERIAL]];
		worldName.mMaterials[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_CAMERA] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_CAMERA);
		worldName.mCameras[cpID] = prefabsWorld->mCameras[prefabCPs[E_CP_CAMERA]];
		worldName.mCameras[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_PLAYER] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_PLAYER);
		worldName.mPlayers[cpID] = prefabsWorld->mPlayers[prefabCPs[E_CP_PLAYER]];
		worldName.mPlayers[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_ANIMATION] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_ANIMATION);
		worldName.mAnimations[cpID] = prefabsWorld->mAnimations[prefabCPs[E_CP_ANIMATION]];
		worldName.mAnimations[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_CIRCLE_COLLIDER] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_CIRCLE_COLLIDER);
		worldName.mCircleColliders[cpID] = prefabsWorld->mCircleColliders[prefabCPs[E_CP_CIRCLE_COLLIDER]];
		worldName.mCircleColliders[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_BOX_COLLIDER] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_BOX_COLLIDER);
		worldName.mPolygonColliders[cpID] = prefabsWorld->mPolygonColliders[prefabCPs[E_CP_BOX_COLLIDER]];
		worldName.mPolygonColliders[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_PARTICLE] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_PARTICLE);
		worldName.mParticles[cpID] = prefabsWorld->mParticles[prefabCPs[E_CP_PARTICLE]];
		worldName.mParticles[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_SCRIPT] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_SCRIPT);
		worldName.mLuaComponents[cpID] = prefabsWorld->mLuaComponents[prefabCPs[E_CP_SCRIPT]];
		worldName.mLuaComponents[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_TAG] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_TAG);
		worldName.mTagComponents[cpID] = prefabsWorld->mTagComponents[prefabCPs[E_CP_TAG]];
		worldName.mTagComponents[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_WIDGET] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_WIDGET);
		worldName.mWidgetComponents[cpID] = prefabsWorld->mWidgetComponents[prefabCPs[E_CP_WIDGET]];
		worldName.mWidgetComponents[cpID].ownerID = entityID;
	}
	if (prefabCPs[E_CP_TEXT] != -1)
	{
		unsigned cpID = CreateComponent(worldName, entityID, E_CP_TEXT);
		worldName.mTextComponents[cpID] = prefabsWorld->mTextComponents[prefabCPs[E_CP_TEXT]];
		worldName.mTextComponents[cpID].ownerID = entityID;
	}
	return entityID;
}


unsigned C_Utility::CreateGameObject(C_World &worldName, const std::string &_name)
{
	int entityID = -1;
	// create from prefab instance
	if (prefabName.find(_name) != prefabName.end())
	{
		debug_func::Report("Copy gameObject from prefab!");
		unsigned targetID = prefabName[_name];
		entityID = CopyGameObject(worldName, targetID);
	}
	else {
		debug_func::Report("Create gameObject from factory");
		entityID = C_ResourceManager::GetInstance()->LoadGameObjects(worldName, _name.c_str());
	}
	return entityID;
}

unsigned C_Utility::RecycleGameObject(C_World &worldName, const std::string &_name)
{
	auto it = worldName.recycledList.begin();
	while (it != worldName.recycledList.end())
	{
		unsigned entityID = *it;
		if (worldName.mEntities[entityID].name == _name)
		{
			it = worldName.recycledList.erase(it);
			worldName.PushNewEntity(entityID);
			worldName.nameToID[_name].insert(entityID);
			debug_func::Report("Recycled " + _name + " from recycle list!");
			return entityID;
		}
		else
			++it;
	}
	debug_func::Report("No "+_name+" in recycle list!");
	return CreateGameObject(worldName, _name);
}

void C_Utility::DestroyEntity(C_World &worldName, unsigned entityID)
{
	DestroyEntity(worldName, entityID, false);
}

void C_Utility::DestroyEntity(C_World &worldName, unsigned entityID, bool recycle)
{
	C_Entity &t = worldName.mEntities[entityID];
	if (t.entityMask == E_CPM_NONE|| t.inDelList) return;
	t.inDelList = true;
	//for (auto i : worldName.newDeledGOs)
	//{
	//	if (i == entityID) return;
	//}
	// break parents relationship
	unsigned trID = worldName.GetComponent(entityID, E_CP_TRANSFORM);
	C_Transform &mTr = worldName.mTransforms[trID];
	// from parent
	if (mTr.parentID != -1)
		worldName.mTransforms[mTr.parentID].childrenID.erase(trID);
	mTr.parentID = -1;
	// let all its children's parent id = -1
	for (auto child : mTr.childrenID)
	{
		worldName.mTransforms[child].parentID = -1;
	}
	mTr.childrenID.clear();
	assert(worldName.mEntities[entityID].mComponents[0] != -1);
	worldName.newDeledGOs.push_back(entityID);
	if (recycle)
	{
		worldName.recycledMark.push_back(true);
	}
	else
	{
		worldName.recycledMark.push_back(false);
	}
}

void C_Utility::DestroyEntityandChildren(C_World &worldName, unsigned entityID)
{
	DestroyEntityandChildren(worldName, entityID, false);
}

void C_Utility::DestroyEntityandChildren(C_World &worldName, unsigned entityID, bool recycle)
{
	C_Entity &t = worldName.mEntities[entityID];
	if (t.entityMask == E_CPM_NONE || t.inDelList) return;
	t.inDelList = true;
	//for (auto i : worldName.newDeledGOs)
	//{
	//	if (i == entityID) return;
	//}
	// break parents relationship
	unsigned trID = worldName.GetComponent(entityID, E_CP_TRANSFORM);
	C_Transform &mTr = worldName.mTransforms[trID];
	// from parent
	if (mTr.parentID != -1)
		worldName.mTransforms[mTr.parentID].childrenID.erase(trID);
	// let all its children's parent id = -1
	std::queue<unsigned> myChildren;
	myChildren.push(trID);
	// push all children into delete list
	while (!myChildren.empty())
	{
		// get the front child
		unsigned nextCh = myChildren.front();
		myChildren.pop();
		// push its children
		for (auto ch : worldName.mTransforms[nextCh].childrenID)
		{
			myChildren.push(ch);
		}
		worldName.mTransforms[nextCh].childrenID.clear();
		worldName.mTransforms[nextCh].parentID = -1;
		assert(worldName.mEntities[nextCh].mComponents[0] != -1);
		//debug_func::Report("push into newDeledGOs, name: " + worldName.mEntities[nextCh].name + " id: " + std::to_string(nextCh));
		worldName.mEntities[nextCh].inDelList = true;
		worldName.newDeledGOs.push_back(nextCh);
		if (recycle)
		{
			worldName.recycledMark.push_back(true);
		}
		else
		{
			worldName.recycledMark.push_back(false);
		}
	}
}

unsigned C_Utility::FindEneity(C_World &worldName, const std::string& name)
{
	if (worldName.nameToID[name].empty())
		return -1;
	return *worldName.nameToID[name].begin();
}

void C_Utility::RemoveComponent(C_World &worldName, unsigned entityID, unsigned cpName)
{
	unsigned& cpID = worldName.mEntities[entityID].mComponents[cpName];
	if (worldName.mEntities[entityID].mComponents[cpName] == -1)
	{
		debug_func::ReportErr("Remove cp Error! No such component in this entity!");
		return;
	}
	worldName.mEntities[entityID].entityMask = worldName.mEntities[entityID].entityMask & (~(std::bitset <64>(1) << cpName));
	//remove owner of this component
	worldName.mComponentsAddresses[cpName][cpID]->ownerID = -1;
	worldName.mComponentsAddresses[cpName][cpID]->Initialize();
	worldName.mDeletedComponents[cpName].push(cpID);
	cpID = -1;
	debug_func::Report("remove " + worldName.mEntities[entityID].name + "'s component"+ ConvertEnumToString(cpName));
}

void C_Utility::LoadModuleInScene(C_World &world, const std::string &moduleName)
{
	std::string modulName = levelRootName + "//Scene//module//" + moduleName + ".json";
	nowmodules = moduleName;
	C_ResourceManager::GetInstance()->LoadScene(world, modulName.c_str());
	nowmodules = "temper";
}

void C_Utility::FreeModuleInScene(C_World &world, const std::string &moduleName)
{
	for (auto i : world.moduleObjects[moduleName])
	{
		DestroyEntity(world, i);
		world.nameToID[world.mEntities[i].name].clear();
	}
	world.moduleObjects[moduleName].clear();
}

void C_Utility::FreeWorld(C_World &world)
{
	for (auto &mModule : world.moduleObjects)
	{
		//do not destroy prefabs and stable objects
		if ( mModule.first != "stable") {
			FreeModuleInScene(world, mModule.first);
		}
	}
	world.recycledList.clear();
}

void C_Utility::InitWorld(C_World &world)
{
	//load level
	std::string fullPath = "..//..//Resource//Json//" + levelRootName + "//Scene//scene.json";
	S_JsonValue value = json_func::LoadJsonData(fullPath.c_str());
	std::vector<std::string> modulNames = value.GetMemberNames();
	for (auto &m : modulNames)
	{
		LoadModuleInScene(world, value[m].asString());
	}
}

void C_Utility::UnloadWorld(C_World &world)
{
	FreeModuleInScene(world, "stable");
	delete prefabsWorld;
	C_ResourceManager::GetInstance()->ClearCurrentSceneResources();
}

void C_Utility::LoadWorld(C_World &world, const std::string &levelName)
{
	levelRootName = levelName;		//filefold name
	prefabsWorld = new C_World(256);
	nowmodules = "prefabs";
	// preload prefabs and resources
	C_ResourceManager::GetInstance()->Preload(levelRootName);

	// load stable
	std::string modulName = levelRootName + "//Scene//stable.json";
	nowmodules = "stable";
	C_ResourceManager::GetInstance()->LoadScene(world, modulName.c_str());
}

void C_Utility::AddChild(C_World &worldName, unsigned parentID, unsigned childID)
{
	if (parentID == -1 || childID == -1) return;
	unsigned parentTRID = worldName.mEntities[parentID].mComponents[E_CP_TRANSFORM];
	unsigned childTRID = worldName.mEntities[childID].mComponents[E_CP_TRANSFORM];
	// if already has this child
	if (worldName.mTransforms[parentTRID].childrenID.find(childTRID) != worldName.mTransforms[parentTRID].childrenID.end())
		return;
	// if this child already has a parent, break that relationship
	if (worldName.mTransforms[childTRID].parentID != -1)
	{
		worldName.mTransforms[worldName.mTransforms[childTRID].parentID].childrenID.erase(childTRID);
	}
	worldName.mTransforms[parentTRID].childrenID.insert(childTRID);
	worldName.mTransforms[childTRID].parentID = parentTRID;
	debug_func::Report("add " + worldName.mEntities[childID].name + " to " + worldName.mEntities[parentID].name+"'s children");
}

void C_Utility::RemoveChild(C_World &worldName, unsigned parentID, unsigned childID)
{
	if (parentID == -1 || childID == -1) return;
	unsigned parentTRID = worldName.mEntities[parentID].mComponents[E_CP_TRANSFORM];
	unsigned childTRID = worldName.mEntities[childID].mComponents[E_CP_TRANSFORM];
	worldName.mTransforms[parentTRID].childrenID.erase(childTRID);
	worldName.mTransforms[childTRID].parentID = -1;
	debug_func::Report("remove " + worldName.mEntities[childID].name +" id: "+ std::to_string(childID)+ " from " + worldName.mEntities[parentID].name + " id: " + std::to_string(parentID) + "'s children");
}

unsigned C_Utility::FindChild(C_World &worldName, unsigned entityID, const std::string &name)
{
	for (auto childTR : worldName.mTransforms[worldName.mEntities[entityID].mComponents[E_CP_TRANSFORM]].childrenID)
	{
		if (worldName.mEntities[worldName.mTransforms[childTR].ownerID].name == name)
		{
			return worldName.mTransforms[childTR].ownerID;
		}
	}
	return -1;
}

unsigned C_Utility::CreateComponent(C_World &worldName, unsigned entityID, unsigned CPNo)
{
	if (worldName.mEntities[entityID].mComponents[CPNo] != -1)
	{
		debug_func::Report(worldName.mEntities[entityID].name + " already has " + ConvertEnumToString(CPNo) + " just serialize it!");
		return worldName.mEntities[entityID].mComponents[CPNo];
	}
	debug_func::Report("creating " + worldName.mEntities[entityID].name + "'s component: " + ConvertEnumToString(CPNo));
	unsigned cp = 0;
	if (worldName.mDeletedComponents[CPNo].empty())
	{
		// full
		if (worldName.mComponentsLastIndex[CPNo] >= worldName.mComponentsAddresses[CPNo].size())
		{
			worldName.resizeComponentList(CPNo);
		}
		cp = worldName.mComponentsLastIndex[CPNo]++;
	}
	else
	{
		cp = worldName.mDeletedComponents[CPNo].front();
		worldName.mDeletedComponents[CPNo].pop();
	}
	worldName.mEntities[entityID].mComponents[CPNo] = cp;
	worldName.mEntities[entityID].entityMask = worldName.mEntities[entityID].entityMask | (std::bitset<64>(1) << CPNo);
	worldName.mComponentsAddresses[CPNo][cp]->ownerID = entityID;
	return cp;
}

unsigned C_Utility::CreateComponent(C_World &worldName, unsigned entityID, const std::string &componentName)
{
	unsigned CPNo = ConvertStringToEnum(componentName, ComponentList);
	return CreateComponent(worldName, entityID, CPNo);
}

void C_Utility::SerializeComponent(C_World &worldName, const std::string &componentName, unsigned cpID, S_JsonValue &value)
{
	debug_func::Report("Serializing " + componentName);
	worldName.mComponentsAddresses[ConvertStringToEnum(componentName, ComponentList)][cpID]->Serialize(value);
}

void C_Utility::InitializeGameobject(C_World &worldName, const std::string &componentName, unsigned entityID, S_JsonValue &value)
{
	// serialize created game object in scene
	debug_func::Report("Initializing " + worldName.mEntities[entityID].name + "'s component "+componentName+" in scene");
	unsigned CPNo = ConvertStringToEnum(componentName, ComponentList);
	worldName.mComponentsAddresses[CPNo][worldName.mEntities[entityID].mComponents[CPNo]]->Serialize(value);
}

C_Utility::C_Utility()
{
}

C_Utility::~C_Utility()
{
}