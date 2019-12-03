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
#include "Component/Transform.h"
C_Transform::C_Transform():parentID(-1), TransMatrix(1.0f), WorldPos(9999.0f), LocalPos(0.0f),Scale(1.0f),WorldScale(1.0f)
{
	LocalQuaternion = WorldQuaternion = GetQuater(vec4(0.0f, 0.0f, 1.0f, 0.0f));
}

C_Transform::~C_Transform()
{

}

void C_Transform::Serialize(S_JsonValue &value)
{
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "position") LocalPos = value[itr].asVec3();
		if (itr == "rotation") {
			vec4 Rotation = value[itr].asVec4();
			Rotation.w = DegreeToRadius(Rotation.w);
			LocalQuaternion = WorldQuaternion = GetQuater(vec4(normalize(vec3(Rotation.x, Rotation.y, Rotation.z)),Rotation.w));
		}
		if (itr == "scale") Scale = value[itr].asVec3();
	}
}

void C_Transform::Initialize()
{
	parentID = -1;
	childrenID.clear();
	TransMatrix = mat4(1.0f);
	WorldPos = vec3(9999.0f,9999.0f, 0.0f);
	LocalPos = vec3(0.0f,0.0f,0.0f);
	LocalQuaternion = WorldQuaternion = GetQuater(vec4(0.0f, 0.0f, 1.0f, 0.0f));
	//WorldRotation = vec4(0.0f);
	Scale = vec3(1.0f);
	WorldScale = vec3(1.0f);
}

float C_Transform::GetWorldRotation2D()
{
	vec3 standard_v = RotationVector(WorldQuaternion, vec3(1.0, 0.0, 0.0));
	return atan2f(standard_v.y, standard_v.x);
}