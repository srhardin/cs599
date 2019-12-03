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
#include "Graphics/RenderPass.h"

using namespace std;
C_RenderPass::C_RenderPass()
{
}

void C_RenderPass::SetSubpassSize(size_t subpass_size)
{
	subpasses.resize(subpass_size);
}

size_t C_RenderPass::S_Subpass::AddParamPair(size_t _material, GLint pos, size_t material_param, size_t mapping_pos_hint)
{
	S_PassMatMap *map = nullptr;
	size_t id = mapping_pos_hint;
	if (materialMappings.size() > 0 && materialMappings[mapping_pos_hint].pMaterial == _material)
	{
		map = &materialMappings[mapping_pos_hint];
	}
	else
	{
		std::vector<S_PassMatMap>::iterator map_itr = materialMappings.begin(), end = materialMappings.end();
		id = 0;
		for (; map_itr != end; ++map_itr)
		{
			if (map_itr->pMaterial == _material)
			{
				map = &(*map_itr);
				break;
			}
			id++;
		}
		if (map == nullptr)
		{
			materialMappings.push_back({ _material });
			map = &materialMappings[materialMappings.size() - 1];
		}
	}
	map->paramPairs.push_back(make_pair(pos, material_param));
	return id;
}