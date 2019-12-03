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
#include "pch.h"
#include "Component/CameraComponent.h"

void C_CameraComponent::Serialize(S_JsonValue & _data)
{
	std::vector<std::string> names = _data.GetMemberNames();
	for (std::string &name : names)
	{
		if (name == "view_name")
		{
			viewName = _data[name].asString();
		}
		else if (name == "ortho")
		{
			ortho = _data[name].asBool();
		}
		else if (name == "keepScreenSize")
		{
			keepScreenSize = _data[name].asBool();
		}
		else if (name == "fov")
		{
			fov = _data[name].asFloat();
		}
		else if (name == "near_plane")
		{
			nearPlane = _data[name].asFloat();
		}
		else if (name == "width_size")
		{
			widthSize = _data[name].asFloat();
		}
	}
}

void C_CameraComponent::Initialize()
{
	viewName = "";
	enabled = true;
	ortho = false;
	keepScreenSize = false;
	aspect = 1.0f;
	fov = 90.0f;
	nearPlane = 0.1f;
	viewMat = mat4(1.0f);
	projectMat = mat4(1.0f);
	viewId = 0;
}