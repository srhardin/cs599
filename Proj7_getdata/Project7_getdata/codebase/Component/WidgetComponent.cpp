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
#include "Component/WidgetComponent.h"

C_WidgetComponent::C_WidgetComponent() : alignCenter(0.0f,0.0f),parentCenter(0.0f,0.0f), offset(0.0f,0.0f), needUpdate(true)
{
}

C_WidgetComponent::~C_WidgetComponent()
{
}

void C_WidgetComponent::Serialize(S_JsonValue & value)
{
	std::vector<std::string> names = value.GetMemberNames();
	for (std::string &itr : names)
	{
		if (itr == "align")
		{
			alignCenter = value[itr].asVec2();
		}
		else if (itr == "parent")
		{
			parentCenter = value[itr].asVec2();
		}
		else if (itr == "offset")
		{
			offset = value[itr].asVec2();
		}
	}
	needUpdate = true;
}

void C_WidgetComponent::Initialize()
{
	needUpdate = true;
}
