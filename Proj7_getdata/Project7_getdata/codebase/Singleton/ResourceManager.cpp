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
#include"Singleton/ResourceManager.h"
#include"Singleton/Utility.h"
#include"Resource/TextureResource.h"
#include"Resource/AudioResource.h"
#include"Resource/MeshResource.h"
#include"Resource/Animator.h"
#include "Entity.h"
#include "Config.h"
#define STB_IMAGE_IMPLEMENTATION
#include"Tools/stb_image.h"


C_ResourceManager::C_ResourceManager()
	:prefabPath("Prefab//"),
	scenePath("Scene//"),
	audioPath("..//..//Resource//Music//"),
	texturePath("..//..//Resource//Texture//"),
	animatorPath("Animator//"),
	assertPath("..//..//Resource//Json//"),
	currentScene(""),
	device(-1),
	freq(14400)
{
	bool initBass = BASS_Init(device, freq, 0, 0, NULL);
	if (!initBass)
	{
		debug_func::ReportErr("Init Bass Error\n");
	}
	C_Config::GetInstance()->SaveSystemVolume();
}

C_ResourceManager::~C_ResourceManager()
{
	debug_func::Report("delete C_ResourceManager");
	for (size_t i = 0; i < S_MeshResource::VertexBuffers.size(); i++)
	{
		glDeleteBuffers(1, &S_MeshResource::VertexBuffers[i]);
	}

	for (size_t i = 0; i < S_MeshResource::NormalBuffers.size(); i++)
	{
		glDeleteBuffers(1, &S_MeshResource::NormalBuffers[i]);
	}

	for (size_t i = 0; i < S_MeshResource::FaceBuffers.size(); i++)
	{
		glDeleteBuffers(1, &S_MeshResource::FaceBuffers[i]);
	}

	for (size_t i = 0; i < S_MeshResource::TextureCoordBuffers.size(); i++)
	{
		glDeleteBuffers(1, &S_MeshResource::TextureCoordBuffers[i]);
	}

	ClearCurrentSceneResources();
}

//void  C_ResourceManager::Init() {
//	Preload("Preload.json");
//}

unsigned int C_ResourceManager::LoadGameObjects(C_World& world,const std::string& prefabName,const std::string& objName) {
	//std::string fullPath = jsonPath + prefabName;
	//S_JsonValue value = json_func::LoadJsonData(fullPath.c_str());


	C_Utility* fac = C_Utility::GetInstance();
	if (prefabName == "")
	{
		return fac->FindEneity(world, objName);
	}

	std::vector<std::string> archetecture = mPrefabs[prefabName].GetMemberNames();
	if (archetecture.size()==0)
	{
		debug_func::ReportErr("has no prefab: " + prefabName +"!");
		return -1;
	}
	std::string goName = objName == "" ? prefabName : objName;

	int entityID = -1;
	bool isAchetype = true;
	bool hasChildren = false;
	for (auto &ar : archetecture)
	{
		if (ar == "archetype")
		{
			isAchetype = false;

			entityID = LoadGameObjects(world, mPrefabs[prefabName]["archetype"].asString(), objName);
			world.mEntities[entityID].name = goName;
			debug_func::Report("Archetype : " + mPrefabs[prefabName]["archetype"].asString() + " Created, continue building " + goName);
			break;
		}
	}

	for (auto &ar : archetecture)
	{
		if (ar == "component")
		{
			std::vector<std::string> cpNames = mPrefabs[prefabName][ar].GetMemberNames();
			if (isAchetype)
			{
				debug_func::Report("Create Achetype " + goName);
				entityID = fac->CreateEntity(world, goName);
			}
			for (auto &cp : cpNames) {

				if (cp == "children")
				{
					hasChildren = true;
					continue;
				}
				int cpID = fac->CreateComponent(world, entityID, cp);
				S_JsonValue tVal = mPrefabs[prefabName]["component"][cp];
				fac->SerializeComponent(world, cp, cpID, tVal);
			}
		}

	}

	if (hasChildren)
	{
		for (int i = 0; i < mPrefabs[prefabName]["component"]["children"].size(); ++i)
		{
			unsigned childId = -1;
			std::vector<std::string> childCpNames = mPrefabs[prefabName]["component"]["children"][i].GetMemberNames();
			for (auto v : childCpNames)
			{
				if (v == "name")
				{
					childId = LoadGameObjects(world, "", mPrefabs[prefabName]["component"]["children"][i]["name"].asString());
					fac->AddChild(world, entityID, childId);
					break;
				}

				if (v == "prefab")
				{
					childId = LoadGameObjects(world, mPrefabs[prefabName]["component"]["children"][i]["prefab"].asString(), "");
					fac->AddChild(world, entityID, childId);
					break;
				}
			}
			for (auto v : childCpNames)
			{
				if (v == "name")
				{
					continue;
				}

				if (v == "prefab")
				{
					continue;
				}
				S_JsonValue tVal = mPrefabs[prefabName]["component"]["children"][i][v];
				fac->InitializeGameobject(world, v, childId, tVal);
			}
		}
	}


	return entityID;
}



void C_ResourceManager::LoadScene(C_World& world, const char* filePath) {
	std::string fullPath = assertPath + filePath;
	S_JsonValue value = json_func::LoadJsonData(fullPath.c_str());
	C_Utility* fac = C_Utility::GetInstance();
	std::vector<std::string> names = value.GetMemberNames();

	for (auto&i : names)
	{
		unsigned int ID = LoadGameObjects(world, value[i]["prefabName"].asString(), i);
		if (ID == -1)
		{
			debug_func::ReportErr("Entity ID equals -1!");
		}
		std::vector<std::string> cp = value[i].GetMemberNames();

		for (auto &n : cp)
		{
			if (n == "prefabName")
			{
				continue;
			}
			S_JsonValue tVal = value[i][n];
			fac->InitializeGameobject(world, n, ID, tVal);
		}
	}
}

const S_TextureResource& C_ResourceManager::GetTexture(const char *filePath) {
	if (mTextures[filePath].w != 0)
	{
		//load to gpu
		if (mTextures[filePath].isLoad2GPU)
		{
			return mTextures[filePath];
		}
		else
		{
			if (mTextures[filePath].Load2GPU())
				return mTextures[filePath];
		}
		debug_func::ReportErr("load to gpu failed./n");
		return mTextures[filePath];
	}

	//not find the texture
	debug_func::ReportErr("do not find the texture./n");
	return mTextures[filePath];
}

const S_AudioResource& C_ResourceManager::GetAudio(const char *filePath) {
	if (mAudios[filePath].mStreamHandle != 0)
	{
		return mAudios[filePath];
	}
	std::string fullPath = audioPath +currentScene+"//"+ filePath;
	S_AudioResource audio(fullPath.c_str());
	mAudios[filePath] = audio;
	return mAudios[filePath];
}

const S_Animator* C_ResourceManager::GetAnimator(const char *filePath) {
	if (mAnimators[filePath].animeSpeed != -1)
	{
		return &mAnimators[filePath];
	}
	std::string fullPath = assertPath +currentScene+"//Animator//"+ filePath + ".json";
	S_JsonValue val = json_func::LoadJsonData(fullPath);
	S_Animator  animator(val);
	mAnimators[filePath] = animator;
	return &mAnimators[filePath];

}

void C_ResourceManager::LoadMeshes() {
	GLuint mVertexBuffer;
	GLuint mNormalBuffer;
	GLuint mFaceBuffer;
	GLuint mTextureCoordBuffer;
	int mFaceCount;
	//add Plane
	C_Plane a;
	int face_count = a.FaceCount();
	//plane vertex buffer
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	int vertex_buffer_size = sizeof(glm::vec4)*a.VertexCount();
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, a.VertexArray(), GL_STATIC_DRAW);

	// plane normal buffer
	glGenBuffers(1, &mNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, a.NormalArray(), GL_STATIC_DRAW);

	// plane face buffer
	glGenBuffers(1, &mFaceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
	int face_buffer_size = sizeof(unsigned int[3])*a.FaceCount();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_buffer_size, a.FaceArray(), GL_STATIC_DRAW);

	// plane coord buffer
	glGenBuffers(1, &mTextureCoordBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextureCoordBuffer);
	int coor_buffer_size = sizeof(vec2) * 4;
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, coor_buffer_size, a.UVArray(), GL_STATIC_DRAW);

	mFaceCount = face_count;

	S_MeshResource::VertexBuffers.push_back(mVertexBuffer);
	S_MeshResource::NormalBuffers.push_back(mNormalBuffer);
	S_MeshResource::FaceBuffers.push_back(mFaceBuffer);
	S_MeshResource::FaceCounts.push_back(mFaceCount);
	S_MeshResource::VertexCount.push_back(a.VertexCount());
	S_MeshResource::TextureCoordBuffers.push_back(mTextureCoordBuffer);


	//add Circle
	C_Circle b;
	glGenBuffers(1, &mVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
	vertex_buffer_size = sizeof(glm::vec4)*b.VertexCount();
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, b.VertexArray(), GL_STATIC_DRAW);

	// Circle normal buffer
	glGenBuffers(1, &mNormalBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, mNormalBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_size, b.NormalArray(), GL_STATIC_DRAW);

	// Circle face buffer
	glGenBuffers(1, &mFaceBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mFaceBuffer);
	face_buffer_size = sizeof(unsigned int[3])*b.FaceCount();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, face_buffer_size, b.FaceArray(), GL_STATIC_DRAW);


	// Circle coord buffer
	glGenBuffers(1, &mTextureCoordBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mTextureCoordBuffer);
	coor_buffer_size = sizeof(vec2) * b.VertexCount();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, coor_buffer_size, b.UVArray(), GL_STATIC_DRAW);

	mFaceCount = b.FaceCount();

	S_MeshResource::VertexBuffers.push_back(mVertexBuffer);
	S_MeshResource::NormalBuffers.push_back(mNormalBuffer);
	S_MeshResource::FaceBuffers.push_back(mFaceBuffer);
	S_MeshResource::FaceCounts.push_back(mFaceCount);
	S_MeshResource::VertexCount.push_back(b.VertexCount());
	S_MeshResource::TextureCoordBuffers.push_back(mTextureCoordBuffer);

	//add MeshResource to mMeshResource
	S_MeshResource pla("plane");
	S_MeshResource cir("circle");
	mMeshes["plane"] = pla;
	mMeshes["circle"] = cir;
}

void C_ResourceManager::LoadTextureToMemory(const std::string& file, const std::string& filePath) {
	unsigned char * image_data = nullptr;
	std::string filepath = file;
	if (file_func::ValidateSuffix(filepath, ".png")) {
		int w = 0, h = 0, c = 0;
		image_data = stbi_load(filePath.c_str(), &w, &h, &c, 4);
		if (c != 4 || image_data == nullptr)
		{
			debug_func::ReportErr("load image" + filepath + " fail or com is not mach, png need com = 4.\n");
		}
		else
		{
			image_data = reinterpret_cast<unsigned char*>(file_func:: vertical_mirror(image_data, c, w, h));
			S_TextureResource tex(image_data, "png", w, h, c);
			mTextures[filepath] = tex;
		}
	}
	else if (file_func::ValidateSuffix(filepath, ".bmp")) {
		int w = 0, h = 0, c = 0;
		image_data = stbi_load(filePath.c_str(), &w, &h, &c, 3);
		if (c != 3 || image_data == nullptr)
		{
			debug_func::ReportErr("load image" + filepath + " fail or com is not mach, bmp need com = 3.\n");
		}
		else
		{
			image_data = static_cast<unsigned char*>(file_func::vertical_mirror(image_data, c, w, h));
			S_TextureResource tex(image_data, "bmp", w, h, c);
			mTextures[filepath] = tex;
		}
	}
	else if (file_func::ValidateSuffix(filepath, ".jpg")) {
		int w = 0, h = 0, c = 0;
		image_data = stbi_load(filePath.c_str(), &w, &h, &c, 3);
		if (c != 3 || image_data == nullptr)
		{
			debug_func::ReportErr("load image" + filepath + " fail or com is not mach, jpg need com = 3.\n");
		}
		else
		{
			image_data = static_cast<unsigned char*>(file_func::vertical_mirror(image_data, c, w, h));
			S_TextureResource tex(image_data, "jpg", w, h, c);
			mTextures[filepath] = tex;
		}
	}
	

}


void C_ResourceManager::Preload(const std::string& folderPath) {
	currentScene = folderPath;
	std::string prefab = assertPath + folderPath+"//" +prefabPath;
	std::string animator = assertPath + folderPath+"//" +animatorPath;
	std::string texture = texturePath + folderPath+"//";
	std::string audio = audioPath + folderPath+"//";

	std::vector<std::string> jsonFilePaths;
	std::vector<std::string> jsons;
	file_func::SearchFilesIncludeSubfolder(prefab, jsons,jsonFilePaths, "json",file_func::E_READFILENAME|file_func::E_READFILEPATH);


	for (int i = 0; i < jsons.size();++i) {
		
		std::string str = jsons[i];
		mPrefabs[str.substr(0, str.rfind("."))] = json_func::LoadJsonData(jsonFilePaths[i]);
	}

	std::vector<std::string> textureFilePaths;
	std::vector<std::string> textures;
	file_func::SearchFilesIncludeSubfolder(texture, textures, textureFilePaths,"png",file_func::E_READFILENAME|file_func::E_READFILEPATH);

	for (int i = 0; i < textures.size(); ++i) {
		LoadTextureToMemory(textures[i], textureFilePaths[i]);
	}


	std::vector<std::string>animatorPaths;
	std::vector<std::string> animators;
	file_func::SearchFilesIncludeSubfolder(animator, animators, animatorPaths,"json",file_func::E_READFILENAME|file_func::E_READFILEPATH);

	for (int i = 0; i < animators.size();++i) {
		
		S_JsonValue val = json_func::LoadJsonData(animatorPaths[i]);
		S_Animator  animator(val);
		mAnimators[animators[i]] = animator;
	}


	std::vector<std::string>audioPaths;
	std::vector<std::string> audios;
	file_func::SearchFilesIncludeSubfolder(audio, audios,audioPaths, "",file_func::E_READFILENAME|file_func::E_READFILEPATH);
	
	for (int i = 0; i < audios.size();++i) {
		if (audios[i] == "." || audios[i] == "..")
			continue;
		S_AudioResource  audio(audioPaths[i].c_str());
		std::string title = audios[i].substr(0, 2);
		if (title =="M_")
		{
			audio.type = E_MUSIC;
		}
		else if (title == "S_")
		{
			audio.type = E_SOUND_EFFET;
		}
		mAudios[audios[i]] = audio;
	}


	for (auto &str : mPrefabs)
	{
	
		C_Utility::GetInstance()->CreatePrefabs(str.first);
	}
}

void C_ResourceManager::ClearCurrentSceneResources() {
	for (auto i = mTextures.begin(); i != mTextures.end(); ++i)
	{
		(*i).second.isLoad2GPU = false;
		glDeleteTextures(1, &(*i).second.texture);
	}

	for (auto e : mAudios)
	{
		BASS_StreamFree(e.second.mStreamHandle);
	}
	mAudios.clear();
	mTextures.clear();
	mPrefabs.clear();
	mAnimators.clear();
}

const S_MeshResource& C_ResourceManager::GetMeshResource(const std::string& name) {
	return mMeshes[name];
}