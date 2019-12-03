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
#include "Tools/Circle.h"
#include"Tools/Mesh.h"
#define DegreeToRad (3.1415926f/180.0f)
C_Circle ::C_Circle()
	: vertices(0),
	normals(0),
	faces(0) {

	float PI = 4.0f * atan(1.0f);
	int Segments = 10;
	//generate data
	circleOutline.resize(Segments+1);
	vertices.resize(Segments+1);
	normals.resize(Segments + 1);
	uv.resize(Segments + 1);
	faces.resize(Segments * 3);
	for (int i = 0; i < Segments; ++i)
	{
		float angle = i * 2.0f * PI / Segments;
		vertices[i] =vec4(cosf(angle), sinf(angle), 0.0f, 1.0f);
		circleOutline[i] = vec4(cosf(angle), sinf(angle), 0.0f, 1.0f);
		uv[i] = vec2(0.5f, 0.5f) + vec2(vertices[i]) * 0.5f;
		normals[i] = vec4(0.0f, 0.0f, 1.0f,0);

		faces[i * 3] = (i + 1) % Segments;
		faces[i * 3 + 1] = Segments;
		faces[i * 3 + 2] = i;


	}
	circleOutline[Segments] = vec4(1.0f, 0.0f, 0.0f, 1.0f);
	vertices[Segments] = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	normals[Segments] = vec4(0.0f, 0.0f, 1.0f,0.0f);
}


int C_Circle::VertexCount(void) {
	return vertices.size();
}


glm::vec4* C_Circle::VertexArray(void) {
	return &(vertices[0]);
}


glm::vec4* C_Circle::NormalArray(void) {
	return &(normals[0]);
}


int C_Circle::FaceCount(void) {
	return faces.size()/3;
}


unsigned int* C_Circle::FaceArray(void) {
	return &(faces[0]);
}


vec2*  C_Circle::UVArray(void) {
	return &(uv[0]);
}

vec4*  C_Circle::CircleOutline(void) {
	return &(circleOutline[0]);
}

int  C_Circle::CircleOutlineCount(void) {
	return circleOutline.size();
}