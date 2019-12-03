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
#include "Component/LuaComponent.h"

C_LuaComponent::C_LuaComponent()
{
}

C_LuaComponent::~C_LuaComponent()
{
}

void C_LuaComponent::Serialize(S_JsonValue & value)
{
	size_t size = value.size();
	names.resize(size);
	for (size_t i = 0; i < size; ++i)
	{
		names[i] = value[i].asString();
	}
}

void C_LuaComponent::Initialize()
{
	names.clear();
}
