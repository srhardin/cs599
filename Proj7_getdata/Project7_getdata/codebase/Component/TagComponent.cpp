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
#include "Component/TagComponent.h"

namespace
{
	std::map<std::string, uint64_t> bit_Library;
	uint64_t new_bit = 1;
	uint64_t GetBit(std::string _name)
	{
		auto bit_itr = bit_Library.find(_name);
		if (bit_itr != bit_Library.end())
		{
			return bit_itr->second;
		}
		else
		{
			bit_Library.insert(std::make_pair(_name, new_bit));
			uint64_t return_val = new_bit;
			new_bit = new_bit << 1;
			return return_val;
		}
	}


}

namespace tagcomp_funcs
{
	uint64_t GetBitSet(std::vector<std::string> const &_name_array)
	{
		uint64_t return_val = 0;
		for (auto &name : _name_array)
		{
			return_val |= GetBit(name);
		}
		return return_val;
	}
}

C_TagComponent::C_TagComponent() : bits(0)
{
}

void C_TagComponent::Serialize(S_JsonValue &_data)
{
	size_t size = _data.size();
	for (size_t i = 0; i < size; ++i)
	{
		bits |= GetBit(_data[i].asString());
	}
}

void C_TagComponent::Initialize()
{
	bits = 0;
}
