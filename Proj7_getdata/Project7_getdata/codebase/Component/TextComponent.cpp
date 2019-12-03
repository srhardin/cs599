/* Start Header -------------------------------------------------------
Copyright (C) 2019
DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior
written consent of DigiPen Institute
of Technology is prohibited.
Language:<C++>
Platform:<windows 10>
Project:<GAM 541, Void Engine>
Author:<Jin Fan>
-End Header --------------------------------------------------------*/
#include "pch.h"
#include "Component/TextComponent.h"

C_TextComponent::C_TextComponent() : text("TEXT"), font("BebasNeue-Regular"), rowInterval(50), textureID(-1), textureWidth(256), textureHeight(256), scale(1.0f,1.0f), rebuild(true)
{
}

void C_TextComponent::Serialize(S_JsonValue &val)
{
	std::vector<std::string> names = val.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "text")
		{
			text = val[itr].asString();
		}
		else if (itr == "font")
		{
			font = val[itr].asString();
		}
	}
	rebuild = true;
}

void C_TextComponent::Initialize()
{
}
