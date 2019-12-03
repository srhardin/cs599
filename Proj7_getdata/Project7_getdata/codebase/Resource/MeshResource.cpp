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
#include"Resource/MeshResource.h"


std::vector<GLuint> S_MeshResource::VertexBuffers;
std::vector<GLuint> S_MeshResource::NormalBuffers;
std::vector<GLuint> S_MeshResource::FaceBuffers;
std::vector<int> S_MeshResource::FaceCounts;
std::vector<int> S_MeshResource::VertexCount;
std::vector<GLuint> S_MeshResource::TextureCoordBuffers;
S_MeshResource::S_MeshResource(std::string type)
	: mVertexBuffer(0)
	, mNormalBuffer(0)
	, mFaceBuffer(0)
	, mFaceCount(0)
	, mTextureCoordBuffer(0)
	, mVertexCount(0) {

	if (type == "plane")
	{
		mVertexBuffer = VertexBuffers[0];
		mNormalBuffer = NormalBuffers[0];
		mFaceBuffer = FaceBuffers[0];
		mFaceCount = FaceCounts[0];
		mVertexCount = VertexCount[0];
		mTextureCoordBuffer = TextureCoordBuffers[0];
	}
	else if (type == "circle")
	{
		mVertexBuffer = VertexBuffers[1];
		mNormalBuffer = NormalBuffers[1];
		mFaceBuffer = FaceBuffers[1];
		mFaceCount = FaceCounts[1];
		mVertexCount = VertexCount[1];
		mTextureCoordBuffer = TextureCoordBuffers[1];

	}

}

S_MeshResource::S_MeshResource()
	: mVertexBuffer(0)
	, mNormalBuffer(0)
	, mFaceBuffer(0)
	, mFaceCount(0)
	, mTextureCoordBuffer(0)
	, mVertexCount(0)
{
}
S_MeshResource::~S_MeshResource() {


}
