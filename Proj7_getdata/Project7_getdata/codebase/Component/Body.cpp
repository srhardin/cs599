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
#include "Component/Body.h"
C_Body::C_Body() :
velocity(0.0f),
//mAcc(0.0f),
totalForce(0.0f),
mass(0.0f),
invMass(0.0f),
restitution(0.0f),
kinematic(false),
velFriction(0.93f),
dynamicFriction(0.5f),
staticFriction(0.5f),
orientation(0),
angularVelocity(0),
torque(0),
enable(true)
{}
C_Body::~C_Body(){}

void C_Body::Serialize(S_JsonValue &value){
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "mass")
		{
			mass = value[itr].asFloat();
			if (0.0f != mass) // a = F * mInvMass
				invMass = 1.0f / mass;
			else // no mass can't move
				invMass = 0.0f;
		}
		if (itr == "restitution") restitution = value[itr].asFloat();
		if (itr == "kinematic") kinematic = value[itr].asBool();
		if (itr == "dFriction") dynamicFriction = value[itr].asFloat();
		if (itr == "sFriction") staticFriction = value[itr].asFloat();
		if (itr == "vFriction") velFriction = value[itr].asFloat();
	}
}

void C_Body::Initialize()
{
	velocity = vec3(0.0f);
	//mAcc = vec3(0.0f);
	totalForce = vec3(0.0f);
	mass = 0.0f;
	invMass = 0.0f;
	restitution = 0.0f;
	kinematic = false;
	velFriction = 0.93f;
	dynamicFriction = 0.1f;
	staticFriction = 0.1f;
	orientation = vec3(0);
	angularVelocity = vec3(0);
	torque = vec3(0);
	enable = true;
}