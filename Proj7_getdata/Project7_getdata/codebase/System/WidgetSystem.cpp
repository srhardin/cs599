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
#include "System/WidgetSystem.h"  
#include "Component/WidgetComponent.h"
#include "System/Renderer.h"
#include "Entity.h"
#include "Enums.h"

C_WidgetSystem::C_WidgetSystem()
{
}

C_WidgetSystem::~C_WidgetSystem()
{
}

void C_WidgetSystem::Register(C_World & _world, int _id)
{
	int wdgt_id = _world.mEntities[_id].mComponents[E_CP_WIDGET];
	C_WidgetComponent &_component = _world.mWidgetComponents[wdgt_id];
	_component.needUpdate = true;
	entities.insert(_id);
}

void C_WidgetSystem::Unregister(C_World & _world, int _id)
{
	entities.erase(_id);
}

void C_WidgetSystem::Update(C_World & _world, bool _force_update)
{
	C_Renderer *_renderer = C_Renderer::globalRenderer;
	vec2 resolution = _renderer->GetResolution();
	vec2 half_res = resolution * 0.5f;
	for (auto _id : entities)
	{
		int wdgt_id = _world.mEntities[_id].mComponents[E_CP_WIDGET];
		int trans_id = _world.mEntities[_id].mComponents[E_CP_TRANSFORM];
		C_WidgetComponent &_component = _world.mWidgetComponents[wdgt_id];
		C_Transform &_transform = _world.mTransforms[trans_id];
		if (_component.needUpdate || _force_update)
		{
			vec2 scale2d = _transform.Scale;
			vec2 parent_pos = _component.parentCenter * half_res;
			vec2 aligned_pos = _component.alignCenter * scale2d;
			vec2 rel_trans = parent_pos - aligned_pos + _component.offset;
			_transform.LocalPos = vec3(rel_trans.x, rel_trans.y, _transform.LocalPos.z);
			_component.needUpdate = false;
		}
	}

}