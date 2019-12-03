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
//#include "Tools/JsonRW.h"
//#include <vector>
//#include "json/value.h"
//#include "Tools/File.h"
//#include "Tools/Debug.h"

namespace
{
	Json::CharReaderBuilder builder;
	Json::CharReader *reader = builder.newCharReader();
}

S_JsonValue::S_JsonValue(Json::Value & val)
{
	value = val;
}
S_JsonValue::S_JsonValue() {
	
}
S_JsonValue S_JsonValue::operator[](std::string key)
{
	return S_JsonValue(value[key]);
}

S_JsonValue S_JsonValue::operator[](int key)
{
	return S_JsonValue(value[key]);
}

const int S_JsonValue::size()
{
	return value.size();
}

bool S_JsonValue::isNull()
{
	return value == Json::nullValue;
}

bool S_JsonValue::isArray()
{
	return value.isArray();
}

int S_JsonValue::asInt()
{
	return value.asInt();
}

float S_JsonValue::asFloat()
{
	return value.asFloat();
}

std::string S_JsonValue::asString()
{
	return value.asString();
}

bool S_JsonValue::asBool()
{
	return value.asBool();
}

//no vector library for now
vec4 S_JsonValue::asVec4()
{
	return vec4(value[0].asFloat(), value[1].asFloat(), value[2].asFloat(), value[3].asFloat());
}

vec3 S_JsonValue::asVec3()
{
	return vec3(value[0].asFloat(), value[1].asFloat(), value[2].asFloat());
}

vec2 S_JsonValue::asVec2()
{
	return vec2(value[0].asFloat(), value[1].asFloat());
}

S_JsonValue S_JsonValue::ExpandSrc()
{
	Json::ValueType type = value.type();
	if (type == Json::objectValue)
	{
		std::vector<std::string> member_names = value.getMemberNames();
		for (std::string &itr : member_names)
		{
			if (itr == SRC_PATH_NAME)
			{
				return json_func::LoadJsonData(value[itr].asString()).ExpandSrc();
			}
			value[itr] = (*this)[itr].ExpandSrc().GetJsonValue();
		}
	}
	else if (type == Json::arrayValue)
	{
		int size = value.size();
		for (int i = 0; i < size; ++i)
		{
			value[i] = (*this)[i].ExpandSrc().GetJsonValue();
		}
	}
	return *this;
}

std::vector<std::string> S_JsonValue::GetMemberNames()
{
	return value.getMemberNames();
}

S_JsonValue json_func::LoadJsonData(std::string path, bool full_path)
{
	if (!full_path)
	{
		//path = FFile::getPath(path);
	}
	std::string raw = file_func::LoadTextFile(path.c_str());
	Json::Value return_val;
	std::string err;
	if (!reader->parse(raw.c_str(), raw.c_str() + raw.size(), &return_val, &err))
	{
		debug_func::ReportErr(err);
	}
	return S_JsonValue(return_val);
}
