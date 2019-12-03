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
#include "Component/MaterialComponent.h"
#include "Singleton/ResourceManager.h"
#include "Resource/TextureResource.h"

void C_MaterialComponent::SetParam(E_PARAM_TYPE type, string _name, void const * _pvalue)
{
	for (size_t i = 0; i < names.size(); ++i)
	{
		if (types[i] == type && names[i] == _name)
		{
			SetIdleParameter(type, pValues[i], _pvalue);
			return;
		}
	}
	size_t id = AssignIdleParameter(type);
	SetIdleParameter(type, id, _pvalue);
	pValues.push_back(id);
	names.push_back(_name);
	types.push_back(type);
	updated = false;
}

size_t C_MaterialComponent::AssignIdleParameter(E_PARAM_TYPE type)
{
	size_t handle = 0;
	AllocateIdleParam(type,paramPool, handle, true);
	return handle + 1;
}

void C_MaterialComponent::FreeIdleParameter(E_PARAM_TYPE type, size_t parameter_handle)
{
	size_t id = parameter_handle - 1;
	AllocateIdleParam(type,paramPool, id, false);
}

void C_MaterialComponent::SetIdleParameter(E_PARAM_TYPE type, size_t parameter_handle, void const* _pvalue)
{
	size_t id = parameter_handle - 1;
	AssignIdleParam(type,paramPool, id, _pvalue);
}

void C_MaterialComponent::Serialize(S_JsonValue &_data)
{
	pValues.clear();
	names.clear();
	types.clear();
	material = _data["name"].asString();
	std::vector<string> pnames = _data.GetMemberNames();
	C_ResourceManager *resource_mgr = C_ResourceManager::GetInstance();
	for (string &name : pnames)
	{
		E_PARAM_TYPE type = E_PARAM_TYPE_MAX;
		if (name == "vec2")
		{
			type = E_PARAM_TYPE_UNIVEC2;
			std::vector<string> params = _data[name].GetMemberNames();
			for (string &param : params)
			{
				//pValues.push_back(new vec2(_data[name][param].asVec2()));
				size_t id = AssignIdleParameter(type);
				SetIdleParameter(type, id, &_data[name][param].asVec2());
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
		else if (name == "vec3")
		{
			type = E_PARAM_TYPE_UNIVEC3;
			std::vector<string> params = _data[name].GetMemberNames();
			for (string &param : params)
			{
				//pValues.push_back(new vec3(_data[name][param].asVec3()));
				size_t id = AssignIdleParameter(type);
				SetIdleParameter(type, id, &_data[name][param].asVec3());
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
		else if (name == "vec4")
		{
			type = E_PARAM_TYPE_UNIVEC4;
			std::vector<string> params = _data[name].GetMemberNames();
			for (string &param : params)
			{
				//pValues.push_back(new vec4(_data[name][param].asVec4()));
				size_t id = AssignIdleParameter(type);
				SetIdleParameter(type, id, &_data[name][param].asVec4());
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
		else if (name == "float")
		{
			type = E_PARAM_TYPE_UNIFLOAT;
			std::vector<string> params = _data[name].GetMemberNames();
			for (string &param : params)
			{
				//pValues.push_back(new float(_data[name][param].asFloat()));
				size_t id = AssignIdleParameter(type);
				GLfloat f = _data[name][param].asFloat();
				SetIdleParameter(type, id, &f);
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
		else if (name == "int")
		{
			type = E_PARAM_TYPE_UNIINT;
			std::vector<string> params = _data[name].GetMemberNames();
			for (string &param : params)
			{
				//pValues.push_back(new int(_data[name][param].asInt()));
				size_t id = AssignIdleParameter(type);
				GLint i = _data[name][param].asInt();
				SetIdleParameter(type, id, &i);
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
		else if (name == "sampler2D")
		{
			type = E_PARAM_TYPE_UNISAMP2D;
			std::vector<string> params = _data[name].GetMemberNames();

			for (string &param : params)
			{
				//pValues.push_back(new GLuint((resource_mgr->GetTexture(_data[name][param].asString().c_str())).texture));
				GLuint i = (resource_mgr->GetTexture(_data[name][param].asString().c_str())).texture;
				size_t id = AssignIdleParameter(type);
				SetIdleParameter(type, id, &i);
				pValues.push_back(id);
				names.push_back(param);
				types.push_back(type);
			}
		}
	}
}

void C_MaterialComponent::Initialize()
{
	pValues.clear();
	names.clear();
	types.clear();
	materialID = materialInstID = 0;
	updated = false;
}