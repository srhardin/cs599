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
#include "Graphics/Pipeline.h"

namespace {
	int gl_shader_type_list[] = { GL_VERTEX_SHADER , GL_FRAGMENT_SHADER };
	char const *shader_type_name_list[] = { "Vertex", "Fragment" , "Unknown" };
	void CollectParamNamesFromShaderSource(std::string& Source, S_ParameterList& param_list, int shader_type)
	{
		std::string right_string = Source;
		while (true)
		{
			int pos = right_string.find("\n");
			if (pos == -1)
			{
				break;
			}
			std::string current_string = right_string.substr(0, pos);
			right_string = right_string.substr(pos + 1);

			std::regex reg("^\\s*uniform\\s+(\\w+)\\s+(\\w+).*");
			std::smatch match;
			if (std::regex_match(current_string, match, reg))
			{
				if (match[1] == "int")
				{
					param_list.names[E_PARAM_TYPE_UNIINT].push_back(match[2]);
				}
				if (match[1] == "float")
				{
					param_list.names[E_PARAM_TYPE_UNIFLOAT].push_back(match[2]);
				}
				else if (match[1] == "vec2")
				{
					param_list.names[E_PARAM_TYPE_UNIVEC2].push_back(match[2]);
				}
				else if (match[1] == "vec3")
				{
					param_list.names[E_PARAM_TYPE_UNIVEC3].push_back(match[2]);
				}
				else if (match[1] == "vec4")
				{
					param_list.names[E_PARAM_TYPE_UNIVEC4].push_back(match[2]);
				}
				else if (match[1] == "mat4")
				{
					param_list.names[E_PARAM_TYPE_UNIMAT4].push_back(match[2]);
				}
				else if (match[1] == "sampler2D")
				{
					param_list.names[E_PARAM_TYPE_UNISAMP2D].push_back(match[2]);
				}
			}
			else if (shader_type == E_SHADER_TYPE_VERTEX)
			{
				std::regex reg_in("^\\s*in\\s+(\\w+)\\s+(\\w+).*");
				std::regex reg_attr("^\\s*attribute\\s+(\\w+)\\s+(\\w+).*");
				if (std::regex_match(current_string, match, reg_in) || std::regex_match(current_string, match, reg_attr))
				{
					if (match[1] == "float")
					{
						param_list.names[E_PARAM_TYPE_ATTFLOAT].push_back(match[2]);
						param_list.names[E_PARAM_TYPE_ATTFLOAT_DETAILED].push_back(match[2]);
					}
					if (match[1] == "vec2")
					{
						param_list.names[E_PARAM_TYPE_ATTVEC2].push_back(match[2]);
					}
					else if (match[1] == "vec3")
					{
						param_list.names[E_PARAM_TYPE_ATTVEC3].push_back(match[2]);
						param_list.names[E_PARAM_TYPE_ATTVEC3_DETAILED].push_back(match[2]);
					}
					else if (match[1] == "vec4")
					{
						param_list.names[E_PARAM_TYPE_ATTVEC4].push_back(match[2]);
						param_list.names[E_PARAM_TYPE_ATTVEC4_DETAILED].push_back(match[2]);
					}
				}
			}
		}
	}

	void combineStringVectors(std::vector<std::string>& target, std::vector<std::string>& first, std::vector<std::string>& second)
	{
		std::map<std::string, bool> map;
		target.clear();
		for (auto itr = first.begin(); itr != first.end(); ++itr)
		{
			if (map.find(*itr) == map.end())
			{
				map.insert(std::pair<std::string, bool>(*itr, true));
				target.push_back(*itr);
			}
		}
		for (auto itr = second.begin(); itr != second.end(); ++itr)
		{
			if (map.find(*itr) == map.end())
			{
				map.insert(std::pair<std::string, bool>(*itr, true));
				target.push_back(*itr);
			}
		}
	}

	void CombineParamNames(S_ParameterList &target, S_ParameterList &first, S_ParameterList &second)
	{
		for (size_t id = 0; id < target.names.size(); ++id)
		{
			combineStringVectors(target.names[id], first.names[id], second.names[id]);
		}
	}
}

bool S_ShaderModule::createShaderFromFile(std::string _name, E_SHADER_TYPE _type)
{
	GLint compile_result;
	std::string shader_source = file_func::LoadTextFile(_name.c_str());

	const char* shader_cstr = shader_source.c_str();
	shaderHandle = glCreateShader(gl_shader_type_list[_type]);
	glShaderSource(shaderHandle, 1, &shader_cstr, 0);
	glCompileShader(shaderHandle);
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &compile_result);
	std::string type_name = shader_type_name_list[_type];
	if (!compile_result)
	{
		char buffer[1024];
		glGetShaderInfoLog(shaderHandle, 1024, 0, buffer);
		debug_func::ReportErr(type_name + " shader " + _name + " creation failed!:\t" + std::string(&buffer[0]));
		return false;
	}
	CollectParamNamesFromShaderSource(shader_source, paramList, _type);
	shaderType = _type;
	return true;
}

C_Pipeline::~C_Pipeline()
{
	FreePipeline();
}

bool C_Pipeline::setVertexShader(S_ShaderModule & vt_shader)
{
	if (vt_shader.shaderType == E_SHADER_TYPE_VERTEX)
	{
		pVertexShader = &vt_shader;
		return true;
	}
	return false;
}

bool C_Pipeline::setFragmentShader(S_ShaderModule & fg_shader)
{
	if (fg_shader.shaderType == E_SHADER_TYPE_FRAGMENT)
	{
		pFragmentShader = &fg_shader;
		return true;
	}
	return false;
}

bool C_Pipeline::buildPipeline()
{
	if (!pVertexShader || !pFragmentShader)
	{
		return false;
	}
	if (program)
	{
		paramList = {};
		paramPoses = {};
		glDeleteProgram(program);
	}
	program = glCreateProgram();
	glAttachShader(program, pVertexShader->shaderHandle);
	glAttachShader(program, pFragmentShader->shaderHandle);
	glLinkProgram(program);
	GLint link_result;
	glGetProgramiv(program, GL_LINK_STATUS, &link_result);
	if (!link_result) {
		char buffer[1024];
		glGetProgramInfoLog(program, 1024, 0, buffer);
		debug_func::ReportErr("Pipeline linking failed!:\t" + std::string(&buffer[0]));
		return false;
	}
	CombineParamNames(this->paramList, pVertexShader->paramList, pFragmentShader->paramList);

#define SET_PARAM( VECNAME , TYPE_FUNC ) paramPoses.positions[##VECNAME##].resize(paramList.names[##VECNAME##].size());\
for(uint16_t itr = 0; itr < paramList.names[##VECNAME##].size(); ++itr )\
{\
	GLint location = TYPE_FUNC(program, paramList.names[##VECNAME##][itr].c_str());\
	paramPoses.positions[##VECNAME##][itr] = location;\
}

	SET_PARAM(E_PARAM_TYPE_UNIINT, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNIFLOAT, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNIVEC2, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNIVEC3, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNIVEC4, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNIMAT4, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_UNISAMP2D, glGetUniformLocation);
	SET_PARAM(E_PARAM_TYPE_ATTFLOAT, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTVEC2, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTVEC3, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTVEC4, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTFLOAT_DETAILED, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTVEC3_DETAILED, glGetAttribLocation);
	SET_PARAM(E_PARAM_TYPE_ATTVEC4_DETAILED, glGetAttribLocation);

#undef SET_PARAM
	return true;
}

void C_Pipeline::FreePipeline()
{
	if (program)
	{
		paramList = {};
		paramPoses = {};
		glDeleteProgram(program);
		program = 0;
	}
	pVertexShader = nullptr;
	pFragmentShader = nullptr;
}

bool C_Pipeline::bindPipeline()
{
	if (program)
	{
		glUseProgram(program);
	}

	return program > 0;
}

GLint C_Pipeline::findParam(std::string _name, E_PARAM_TYPE _type)
{
	if (_type < E_PARAM_TYPE_MAX)
	{
		std::vector<GLint> &param_poses = paramPoses.positions[_type];
		std::vector<std::string> &param_names = paramList.names[_type];
		for (uint16_t id = 0; id < param_names.size(); ++id)
		{
			if (param_names[id] == _name)
			{
				return param_poses[id];
			}
		}
	}
	return -1;
}