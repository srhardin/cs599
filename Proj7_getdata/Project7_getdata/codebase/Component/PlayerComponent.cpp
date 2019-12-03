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
#include "Component/PlayerComponent.h"

C_PlayerComponent::C_PlayerComponent() :playerID(1), commandList(0),moveDirection(vec2(0.0f,0.0f)), moveMagnitude(0.0f), orientation(vec2(0.0f, 0.0f)), turnMagnitude(0.0f)
{

}
C_PlayerComponent::~C_PlayerComponent()
{

}

void C_PlayerComponent::Serialize(S_JsonValue &value)
{
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "playerID") playerID = value[itr].asInt();
	}
}

void C_PlayerComponent::Initialize()
{
	playerID = 1;
	commandList = 0;
	moveDirection = vec2(0.0f,0.0f);
	moveMagnitude = 0.0f;
	orientation = vec2(0.0f,0.0f);
	turnMagnitude = 0.0f;
}