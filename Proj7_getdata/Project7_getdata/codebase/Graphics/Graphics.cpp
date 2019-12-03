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
#include "Graphics/Graphics.h"

using namespace graphics;
namespace
{
	void AllocateFreeUInt(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		if (_alloc)
		{
			handle = _pool.int_uniform.assignData();
		}
		else
		{
			_pool.int_uniform.freeData(handle);
		}
	}

	void AllocateFreeUFloat(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.float_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.float_uniform.assignData();
		}
		else
		{
			_pool.float_uniform.freeData(handle);
		}
	}

	void AllocateFreeUVec2(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec2_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.vec2_uniform.assignData();
		}
		else
		{
			_pool.vec2_uniform.freeData(handle);
		}
	}

	void AllocateFreeUVec3(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec3_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.vec3_uniform.assignData();
		}
		else
		{
			_pool.vec3_uniform.freeData(handle);
		}
	}

	void AllocateFreeUVec4(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec4_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.vec4_uniform.assignData();
		}
		else
		{
			_pool.vec4_uniform.freeData(handle);
		}
	}

	void AllocateFreeUMat4(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.mat4_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.mat4_uniform.assignData();
		}
		else
		{
			_pool.mat4_uniform.freeData(handle);
		}
	}

	void AllocateFreeUSamp2D(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.sampler2D_uniform.assignData();
		if (_alloc)
		{
			handle = _pool.sampler2D_uniform.assignData();
		}
		else
		{
			_pool.sampler2D_uniform.freeData(handle);
		}
	}

	void AllocateFreeAVec2(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec2_attribute.assignData();
		if (_alloc)
		{
			handle = _pool.vec2_attribute.assignData();
		}
		else
		{
			_pool.vec2_attribute.freeData(handle);
		}
	}

	void AllocateFreeAVec3(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec3_attribute.assignData();
		if (_alloc)
		{
			handle = _pool.vec3_attribute.assignData();
		}
		else
		{
			_pool.vec3_attribute.freeData(handle);
		}
	}

	void AllocateFreeAVec4(S_ParameterPool &_pool, size_t &handle, bool _alloc)
	{
		//return _pool.vec4_attribute.assignData();
		if (_alloc)
		{
			handle = _pool.vec4_attribute.assignData();
		}
		else
		{
			_pool.vec4_attribute.freeData(handle);
		}
	}

	void AssignUInt(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		//_pool.int_uniform.data[handle] = *reinterpret_cast<GLint const *>(param);
		memcpy(&_pool.int_uniform.data[handle], param, sizeof(GLint));
	}

	void AssignUFloat(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.float_uniform.data[handle], param, sizeof(GLfloat));
	}

	void AssignUVec2(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec2_uniform.data[handle], param, sizeof(vec2));
	}

	void AssignUVec3(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec3_uniform.data[handle], param, sizeof(vec3));
	}

	void AssignUVec4(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec4_uniform.data[handle], param, sizeof(vec4));
	}

	void AssignUMat4(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.mat4_uniform.data[handle], param, sizeof(mat4));
	}

	void AssignUSamp2D(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.sampler2D_uniform.data[handle], param, sizeof(GLuint));
	}

	void AssignAVec2(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec2_attribute.data[handle], param, sizeof(GLuint));
	}

	void AssignAVec3(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec3_attribute.data[handle], param, sizeof(GLuint));
	}

	void AssignAVec4(S_ParameterPool &_pool, size_t &handle, void const *param)
	{
		memcpy(&_pool.vec4_attribute.data[handle], param, sizeof(GLuint));
	}

	void *GetUInt(S_ParameterPool &_pool, size_t handle)
	{
		return &_pool.int_uniform.data[handle];
	}

	void *GetUFloat(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.float_uniform.data[handle], param, sizeof(GLfloat));
		return &_pool.float_uniform.data[handle];
	}

	void *GetUVec2(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec2_uniform.data[handle], param, sizeof(vec2));
		return &_pool.vec2_uniform.data[handle];
	}

	void *GetUVec3(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec3_uniform.data[handle], param, sizeof(vec3));
		return &_pool.vec3_uniform.data[handle];
	}

	void *GetUVec4(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec4_uniform.data[handle], param, sizeof(vec4));
		return &_pool.vec4_uniform.data[handle];
	}

	void *GetUMat4(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.mat4_uniform.data[handle], param, sizeof(mat4));
		return &_pool.mat4_uniform.data[handle];
	}

	void *GetUSamp2D(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.sampler2D_uniform.data[handle], param, sizeof(GLuint));
		return &_pool.sampler2D_uniform.data[handle];
	}

	void *GetAVec2(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec2_attribute.data[handle], param, sizeof(GLuint));
		return &_pool.vec2_attribute.data[handle];
	}

	void *GetAVec3(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec3_attribute.data[handle], param, sizeof(GLuint));
		return &_pool.vec3_attribute.data[handle];
	}

	void *GetAVec4(S_ParameterPool &_pool, size_t handle)
	{
		//memcpy(&_pool.vec4_attribute.data[handle], param, sizeof(GLuint));
		return &_pool.vec4_attribute.data[handle];
	}

	void(*IdleParamAllocationFunctions[E_PARAM_TYPE_MAX])(S_ParameterPool &, size_t &, bool) =
	{
		AllocateFreeUInt,
		AllocateFreeUFloat,
		AllocateFreeUVec2,
		AllocateFreeUVec3,
		AllocateFreeUVec4,
		AllocateFreeUMat4,
		AllocateFreeUSamp2D,
		//AllocateFreeAVec2,
		//AllocateFreeAVec3,
		//AllocateFreeAVec4
	};

	void(*IdleParamAssignFunctions[E_PARAM_TYPE_MAX])(S_ParameterPool &_pool, size_t &handle, void const *param) =
	{
		AssignUInt,
		AssignUFloat,
		AssignUVec2,
		AssignUVec3,
		AssignUVec4,
		AssignUMat4,
		AssignUSamp2D,
		//AssignAVec2,
		//AssignAVec3,
		//AssignAVec4,
	};

	void *(*IdleParamGetFunctions[E_PARAM_TYPE_MAX])(S_ParameterPool &_pool, size_t handle) =
	{
		GetUInt,
		GetUFloat,
		GetUVec2,
		GetUVec3,
		GetUVec4,
		GetUMat4,
		GetUSamp2D,
		//GetAVec2,
		//GetAVec3,
		//GetAVec4
	};

	std::string BuildInParamNames[E_BUILDIN_MAX] =
	{
		"BUILDIN_POS",
		"BUILDIN_TEXCOORD",
		"BUILDIN_NORMAL",
		"BUILDIN_UV_OFFSET",
		"BUILDIN_UV_SCALE",
		"BUILDIN_MODEL_MAT",
		"BUILDIN_COLLIDER_REL_MAT",
		"BUILDIN_PARTICLE_INITIAL_LIFETIME",
		"BUILDIN_PARTICLE_SIZE",
		"BUILDIN_PARTICLE_POS",
		"BUILDIN_PARTICLE_VEL",
		"BUILDIN_PARTICLE_LIFETIME",
		"BUILDIN_PARTICLE_POS_DETAILED",
		"BUILDIN_PARTICLE_VEL_DETAILED",
		"BUILDIN_PARTICLE_LIFETIME_DETAILED",
		"BUILDIN_TEX_SCALE",
		"BUILDIN_TEX_SAMP",
	};
}

std::string graphics::GetBuildInParamName(E_BUILD_IN_PARAM param)
{
	return BuildInParamNames[param];
}

void graphics::AllocateIdleParam(E_PARAM_TYPE type, S_ParameterPool & _pool, size_t & handle, bool _alloc)
{
	IdleParamAllocationFunctions[type](_pool, handle, _alloc);
}

void graphics::AssignIdleParam(E_PARAM_TYPE type, S_ParameterPool & _pool, size_t & handle, void const * param)
{
	IdleParamAssignFunctions[type](_pool, handle, param);
}

void * graphics::GetIdleParam(E_PARAM_TYPE type, S_ParameterPool & _pool, size_t handle)
{
	return IdleParamGetFunctions[type](_pool, handle);
}

GLint graphics::CreateGLBuffer()
{
	return GLint();
}
