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
#include "Tools/Plane.h"
C_Plane::C_Plane()
    : vertices(0),
      normals(0),
      faces(0) {
	vec4 a(-1,-1,0,1);
	vec4 b(1, -1, 0, 1);
	vec4 c(1, 1, 0, 1);
	vec4 d(-1, 1, 0, 1);


	vec4 Positions[4] = {
{-1.0f,1.0f,0.0f,1.0f},
{-1.0f,-1.0f,0.0f,1.0f},
{1.0f,-1.0f,0.0f,1.0f},
{1.0f,1.0f,0.0f,1.0f}
	};
	vec3 Normals[4] = {
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
	};
	vec2 UVs[4] = {
		{0.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f}
	};
	int Indices[6] = { 0,1,3,3,1,2 };
	vertices.push_back(Positions[0]);
	vertices.push_back(Positions[1]);
	vertices.push_back(Positions[2]);
	vertices.push_back(Positions[3]);
	glm::vec4 normal(0, 0, 1, 0);
	for (size_t i = 0; i < vertices.size(); i++)
	{
		normals.push_back(normal);
	}

	uv.push_back(UVs[0]);
	uv.push_back(UVs[1]);
	uv.push_back(UVs[2]);
	uv.push_back(UVs[3]);

	for (size_t i = 0; i < 6; i++)
	{
		faces.push_back(Indices[i]);
	}
}

int C_Plane::VertexCount(void) {
	return vertices.size();
}


vec4* C_Plane::VertexArray(void) {
	return &(vertices[0]);
}


vec4* C_Plane::NormalArray(void) {
	return &(normals[0]);
}


int C_Plane::FaceCount(void) {
	return faces.size()/3;
}


unsigned int* C_Plane::FaceArray(void) {
	return &(faces[0]);
}


vec2* C_Plane::UVArray(void) {
	
	return &(uv[0]);
}