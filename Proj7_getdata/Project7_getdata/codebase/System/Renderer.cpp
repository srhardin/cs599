#include "pch.h"
#include "Enums.h"
#include "System/Renderer.h"
#include "Tools/Window.h"
#include "Tools/VoidMath.h"
#include "Entity.h"
#include "Singleton/ResourceManager.h"
#include "Resource/TextureResource.h"
#include"Component/MeshComponent.h"
#include "Resource/MeshResource.h"
#include "Component/Transform.h"
#include "Component/AnimateComponent.h"
#include "Component/MaterialComponent.h"
#include "Component/CameraComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TextComponent.h"
#include "Component/Collider.h"
#include <thread>

#define GRAPHICS_FOLDER "Graphics/"
#define ENABLE_PARTICLES true

using namespace graphics;
namespace
{
	vec4 Positions[4] = {
{-1.0f,1.0f,0.0f,1.0f},
{-1.0f,-1.0f,0.0f,1.0f},
{1.0f,-1.0f,0.0f,1.0f},
{1.0f,1.0f,0.0f,1.0f}
	};
	vec3 Normals[4] = {
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
		{0.0f,0.0f,1.0f},
	};
	vec2 UVs[4] = {
		{0.0f,1.0f},
		{0.0f,0.0f},
		{1.0f,0.0f},
		{1.0f,1.0f}
	};
	int Indices[6] = { 0,1,3,3,1,2 };
	GLuint BufferIDs[4] = { 0 };

	mat4 default_mat(1.0f);

	const float PI = atanf(1.0f) * 4.0f;
	const float onedevPI = 1.0f / PI;

	uint16_t textBindPoint = 0;

	void GLSetUniformInt(GLint location, GLsizei count, void const *data)
	{
		glUniform1iv(location, count, reinterpret_cast<GLint const*>(data));
	}

	void GLSetUniformFloat(GLint location, GLsizei count, void const *data)
	{
		glUniform1fv(location, count, reinterpret_cast<GLfloat const*>(data));
	}

	void GLSetUniformVec2(GLint location, GLsizei count, void const *data)
	{
		glUniform2fv(location, count, reinterpret_cast<const GLfloat*>(data));
	}

	void GLSetUniformVec3(GLint location, GLsizei count, void const *data)
	{
		glUniform3fv(location, count, reinterpret_cast<const GLfloat*>(data));
	}

	void GLSetUniformVec4(GLint location, GLsizei count, void const *data)
	{
		glUniform4fv(location, count, reinterpret_cast<const GLfloat*>(data));
	}

	void GLSetUniformMat4(GLint location, GLsizei count, void const *data)
	{
		glUniformMatrix4fv(location, count, false, reinterpret_cast<GLfloat const*>(data));
	}

	void GLSetUniformSamp2D(GLint location, GLsizei count, void const *data)
	{
		glUniform1i(location, textBindPoint);
		glActiveTexture(GL_TEXTURE0 + textBindPoint);
		GLuint i = (*reinterpret_cast<const GLuint*>(data));
		glBindTexture(GL_TEXTURE_2D, i);
		++textBindPoint;
	}

	void GLSetAttribFloat(GLint location, GLsizei count, void const *data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(data));
		glVertexAttribPointer(location, 1, GL_FLOAT, false, 0, 0);
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribVec2(GLint location, GLsizei count, void const *data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(data));
		glVertexAttribPointer(location, 2, GL_FLOAT, false, 0, 0);
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribVec3(GLint location, GLsizei count, void const *data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(data));
		glVertexAttribPointer(location, 3, GL_FLOAT, false, 0, 0);
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribVec4(GLint location, GLsizei count, void const *data)
	{
		glBindBuffer(GL_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(data));
		glVertexAttribPointer(location, 4, GL_FLOAT, false, 0, 0);
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribFloatDetailed(GLint location, GLsizei count, void const *data)
	{
		S_AttributeBufferInfo const *info = reinterpret_cast<const S_AttributeBufferInfo*>(data);
		glBindBuffer(GL_ARRAY_BUFFER, info->buffer);
		int offset = info->offset * sizeof(GLfloat);
		glVertexAttribPointer(location, 1, GL_FLOAT, false, 0, reinterpret_cast<void*>(offset));
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribVec3Detailed(GLint location, GLsizei count, void const *data)
	{
		S_AttributeBufferInfo const *info = reinterpret_cast<const S_AttributeBufferInfo*>(data);
		glBindBuffer(GL_ARRAY_BUFFER, info->buffer);
		int offset = info->offset * 3 * sizeof(GLfloat);
		glVertexAttribPointer(location, 3, GL_FLOAT, false, 0, reinterpret_cast<void*>(offset));
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void GLSetAttribVec4Detailed(GLint location, GLsizei count, void const *data)
	{
		S_AttributeBufferInfo const *info = reinterpret_cast<const S_AttributeBufferInfo*>(data);
		glBindBuffer(GL_ARRAY_BUFFER, info->buffer);
		int offset = info->offset * 4 * sizeof(GLfloat);
		glVertexAttribPointer(location, 4, GL_FLOAT, false, 0, reinterpret_cast<const void*>(&offset));
		glVertexAttribDivisor(location, count);
		glEnableVertexAttribArray(location);
	}

	void(*ParamSetFunctions[E_PARAM_TYPE_MAX])(GLint location, GLsizei count, void const *data) =
	{
		GLSetUniformInt,
		GLSetUniformFloat,
		GLSetUniformVec2,
		GLSetUniformVec3,
		GLSetUniformVec4,
		GLSetUniformMat4,
		GLSetUniformSamp2D,
		GLSetAttribFloat,
		GLSetAttribVec2,
		GLSetAttribVec3,
		GLSetAttribVec4,
		GLSetAttribFloatDetailed,
		GLSetAttribVec3Detailed,
		GLSetAttribVec4Detailed,
	};

	void PassOperationClearColor()
	{
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void PassOperationClearDepth()
	{
		glClear(GL_DEPTH_BUFFER_BIT);
	}

	void PassOperationClearColorDepth()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void PassOperationDisableDepth()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void PassOperationEnableDepth()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void PassOperationBlendAcc()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	}

	void PassOperationBlendInterp()
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void PassOperationPolygonModeFill()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	void PassOperationPolygonModeLine()
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	void(*PassOperations[E_OP_MAX])() =
	{
		PassOperationClearColor,
		PassOperationClearDepth,
		PassOperationClearColorDepth,
		PassOperationDisableDepth,
		PassOperationEnableDepth,
		PassOperationBlendAcc,
		PassOperationBlendInterp,
		PassOperationPolygonModeFill,
		PassOperationPolygonModeLine
	};
}

C_Renderer *C_Renderer::globalRenderer = nullptr;

C_Renderer::C_Renderer() : needUpdateSize(true), gpuParticleSystem(*this), frameBuffersPool(2), pipelinePool(5), renderPassesPool(2), materialsPool(10), viewPool(2), currentRenderpassConfig(0), ComputeShaderSupport(false)
{
	globalRenderer = this;
}

C_Renderer::~C_Renderer()
{
}

void C_Renderer::Init()
{
	debug_func::CheckGlewErr(glewInit());
	GLint n;
	glGetIntegerv(GL_NUM_EXTENSIONS, &n);
	for (GLint i = 0; i < n; ++i)
	{
		std::string _ext(reinterpret_cast<const char*>(glGetStringi(GL_EXTENSIONS, i)));
		if (_ext == "GL_ARB_compute_shader")
		{
			ComputeShaderSupport = true;
		}
	}
	//glEnable(GL_DEPTH_TEST);
	//glEnable(GL_TEXTURE);
	//glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthMask(GL_TRUE);
	glShadeModel(GL_SMOOTH);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glViewport(0, 0, static_cast<GLsizei>(outputWindow->width), static_cast<GLsizei>(outputWindow->height));
	//Enable Blend;
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glClearDepth(0);
	glEnable(GL_DEPTH_FUNC);
	glDepthFunc(GL_GREATER);

	SetupDefaultRenderer();

	debugMaterial = GetMaterial("DebugMaterial");

	glGenBuffers(4, BufferIDs);
	glBindBuffer(GL_ARRAY_BUFFER, BufferIDs[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Positions), &Positions[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferIDs[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Normals), &Normals[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, BufferIDs[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(UVs), &UVs[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIDs[3]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), &Indices[0], GL_STATIC_DRAW);

	gpuParticleSystem.Init();

	txtSys.Init();
}

size_t C_Renderer::AddPipeline(string _name, string vtx_shader, string frag_shader)
{
	S_ShaderModule *vert_module = loadShader(file_func::GetPath(GRAPHICS_FOLDER + vtx_shader)),
		*frag_module = loadShader(file_func::GetPath(GRAPHICS_FOLDER + frag_shader));
	if (vert_module && frag_module)
	{
		size_t handle = pipelinePool.assignData();
		pipelinePool.data[handle].setVertexShader(*vert_module);
		pipelinePool.data[handle].setFragmentShader(*frag_module);
		pipelinePool.data[handle].buildPipeline();
		pPipelines.insert(make_pair(_name, handle));
		return handle + 1;
	}
	return 0;
}

size_t C_Renderer::GetPipeline(string _name)
{
	map<string, size_t>::iterator result = pPipelines.find(_name);
	if (result != pPipelines.end())
	{
		return result->second + 1;
	}
	return 0;
}

size_t C_Renderer::AddFrameBuffer(string _name)
{
	size_t new_id = frameBuffersPool.assignData();
	pFrameBuffers.insert(make_pair(_name, new_id));
	return new_id + 1;
}

size_t C_Renderer::GetFrameBuffer(string _name)
{
	std::map<string, size_t>::iterator result = pFrameBuffers.find(_name);
	if (result != pFrameBuffers.end())
	{
		return result->second + 1;
	}
	return 0;
}

void C_Renderer::SetFrameBufferStruct(size_t _frame_buffer_handle, std::vector<E_TEXTURE_BUFFER_TYPE> _structs)
{
	C_FrameBuffer &frame_buffer = frameBuffersPool.data[_frame_buffer_handle - 1];
	size_t texture_size = _structs.size();
	frame_buffer.mTextures.resize(texture_size);
	for (size_t i = 0; i < texture_size; ++i)
	{
		frame_buffer.mTextures[i].type = _structs[i];
		if (frame_buffer.mTextures[i].type < E_TYPE_OUTPUT_COLOR)
		{
			GLuint new_tex = 0;
			glGenTextures(1, &new_tex);
			glBindTexture(GL_TEXTURE_2D, new_tex);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
			switch (frame_buffer.mTextures[i].type)
			{
			case E_TTYPE_RGBA:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				break;
			case E_TTYPE_RGB:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
				break;
			case E_TTYPE_DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
				break;
			default:
				break;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
			frame_buffer.mTextures[i].textureID = new_tex;
		}
	}
}

void C_Renderer::RefreshFrameBufferStruct(size_t _frame_buffer_handle)
{
	C_FrameBuffer &frame_buffer = frameBuffersPool.data[_frame_buffer_handle - 1];
	size_t texture_size = frame_buffer.mTextures.size();
	for (size_t i = 0; i < texture_size; ++i)
	{
		if (frame_buffer.mTextures[i].type < E_TYPE_OUTPUT_COLOR)
		{
			glBindTexture(GL_TEXTURE_2D, frame_buffer.mTextures[i].textureID);
			switch (frame_buffer.mTextures[i].type)
			{
			case E_TTYPE_RGBA:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0);
				break;
			case E_TTYPE_RGB:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
				break;
			case E_TTYPE_DEPTH:
				glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT16, width, height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, 0);
				break;
			default:
				break;
			}
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
}

size_t C_Renderer::AddMaterial(string _name)
{
	size_t id = materialsPool.assignData();
	pMaterials.insert(make_pair(_name, id));
	return id + 1;
}

size_t C_Renderer::GetMaterial(string _name)
{
	map<string, size_t>::iterator result = pMaterials.find(_name);
	if (result != pMaterials.end())
	{
		return result->second + 1;
	}
	return 0;
}

size_t C_Renderer::AddMaterialParameter(size_t _handle, string _param_name, E_PARAM_TYPE _param_type)
{
	if (_handle > 0)
	{
		C_Material &mat = materialsPool.data[_handle - 1];
		return mat.AddParameterData(_param_type, _param_name);
	}
	return 0;
}

size_t C_Renderer::GetMaterialParameter(size_t _mat_handle, string _param_name, E_PARAM_TYPE _param_type)
{
	if (_mat_handle > 0)
	{
		C_Material &mat = materialsPool.data[_mat_handle - 1];
		return mat.GetParameterData(_param_type, _param_name);
	}
	return 0;
}

void C_Renderer::SetMaterialParameterDefaultValue(size_t _mat_handle, size_t parameter_handle, uint32_t _size, void const * _p_data, bool _self_manage)
{
	C_Material &mat = materialsPool.data[_mat_handle - 1];
	mat.SetParameterDefaultValue(parameter_handle, _size, _p_data, _self_manage);
}

size_t C_Renderer::AddMaterialInstance(size_t _mat_handle)
{
	if (_mat_handle > 0)
	{
		return materialsPool.data[_mat_handle - 1].AddMaterialInstance();
	}
	return 0;
}

void C_Renderer::DestroyMaterialInstance(size_t _mat_handle, size_t _inst_handle)
{
	if (_mat_handle > 0)
	{
		materialsPool.data[_mat_handle - 1].DestroyMaterialInstance(_inst_handle);
	}
}

void C_Renderer::SetMaterialInstanceParamValue(size_t _mat_handle, size_t _instance_handle, size_t parameter_handle, uint32_t _size, void const * _p_data, bool _self_manage)
{
	if (_mat_handle > 0 && parameter_handle > 0)
	{
		materialsPool.data[_mat_handle - 1].SetInstanceParameter(_instance_handle, parameter_handle, _size, _p_data, _self_manage);
	}
}

void C_Renderer::SetMaterialDrawIndices(size_t _mat_handle, void const * p_data, size_t size)
{
	C_Material &mat = materialsPool.data[_mat_handle - 1];
	mat.SetParameterDefaultValue(0, size, p_data, false);
}

void C_Renderer::SetMaterialInstanceDrawIndices(size_t _mat_handle, size_t _instance_handle, void const * p_data, size_t size)
{
	C_Material &mat = materialsPool.data[_mat_handle - 1];
	mat.SetInstanceParameter(_instance_handle, 0, size, p_data, false);
}

void C_Renderer::SetMaterialInstanceInstNum(size_t _mat_handle, size_t _instance_handle, int *_num)
{
	C_Material &mat = materialsPool.data[_mat_handle - 1];
	mat.SetInstanceInstNum(_instance_handle, _num);
}

size_t C_Renderer::GetView(string _name)
{
	map<string, size_t>::iterator result = pViews.find(_name);
	if (result != pViews.end())
	{
		return result->second + 1;
	}
	//no such view exist, add a new one
	size_t id = viewPool.assignData();
	pViews.insert(make_pair(_name, id));
	return id + 1;
}

void C_Renderer::SetView(size_t _view_handle, void const * p_view_matrix, void const * p_proj_matrix)
{
	if (_view_handle > 0)
	{
		S_View &view = viewPool.data[_view_handle - 1];
		view.pViewMatrix = reinterpret_cast<GLfloat const *>(p_view_matrix);
		view.pProjectMatrix = reinterpret_cast<GLfloat const *>(p_proj_matrix);
	}
}

vec2 C_Renderer::GetResolution()
{
	return vec2(static_cast<float>(width), static_cast<float>(height));
}

S_ShaderModule *C_Renderer::loadShader(string _shader_name)
{
	map<string, S_ShaderModule>::iterator itr = shaderModules.find(_shader_name);
	if (itr != shaderModules.end())
	{
		return &(itr->second);
	}

	itr = shaderModules.insert(make_pair(_shader_name, S_ShaderModule())).first;
	string suffix = file_func::GetSuffix(_shader_name);
	E_SHADER_TYPE type = E_SHADER_TYPE_MAX;
	if (suffix == ".vert")
	{
		type = E_SHADER_TYPE_VERTEX;
	}
	else if (suffix == ".frag")
	{
		type = E_SHADER_TYPE_FRAGMENT;
	}
	else
	{
		debug_func::ReportErr("Invalid shader file type " + suffix);
		return nullptr;
	}
	itr->second.createShaderFromFile(_shader_name, type);
	return &(itr->second);
}

#ifdef _WIN32
void C_Renderer::initWin32WindowContext()
{
	win32Device = GetDC(outputWindow->GetWin32Handle());

	static PIXELFORMATDESCRIPTOR pixel_descriptor{};
	pixel_descriptor.nSize = sizeof(PIXELFORMATDESCRIPTOR);
	pixel_descriptor.nVersion = 1;
	pixel_descriptor.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pixel_descriptor.iPixelType = PFD_TYPE_RGBA;
	pixel_descriptor.cColorBits = 32;
	pixel_descriptor.cRedBits = 0;
	pixel_descriptor.cRedShift = 0;
	pixel_descriptor.cGreenBits = 0;
	pixel_descriptor.cGreenShift = 0;
	pixel_descriptor.cBlueBits = 0;
	pixel_descriptor.cBlueShift = 0;
	pixel_descriptor.cAlphaBits = 0;
	pixel_descriptor.cAlphaShift = 0;
	pixel_descriptor.cAccumBits = 0;
	pixel_descriptor.cAccumRedBits = 0;
	pixel_descriptor.cAccumGreenBits = 0;
	pixel_descriptor.cAccumBlueBits = 0;
	pixel_descriptor.cAccumAlphaBits = 0;
	pixel_descriptor.cDepthBits = 24;
	pixel_descriptor.cStencilBits = 8;
	pixel_descriptor.cAuxBuffers = 0;
	pixel_descriptor.iLayerType = PFD_MAIN_PLANE;
	pixel_descriptor.bReserved = 0;
	pixel_descriptor.dwLayerMask = 0;
	pixel_descriptor.dwVisibleMask = 0;
	pixel_descriptor.dwDamageMask = 0;

	int pixel_format = ChoosePixelFormat(win32Device, &pixel_descriptor);
	debug_func::CheckErr(SetPixelFormat(win32Device, pixel_format, &pixel_descriptor), "Pixel Format Set Failed!");
	win32RenderContext = wglCreateContext(win32Device);

	debug_func::CheckErr(wglMakeCurrent(win32Device, win32RenderContext), "Render Context Failed!");

}
#endif //_WIN32

void C_Renderer::RegisterWindow(C_Window & _window)
{
	outputWindow = &_window;
	width = _window.width;
	height = _window.height;
#ifdef _WIN32
	initWin32WindowContext();
#endif //_WIN32
}

C_Window * C_Renderer::GetOutputWindow()
{
	return outputWindow;
}

bool C_Renderer::isWindowActivated()
{
	if (outputWindow != nullptr)
	{
		return outputWindow->Activated;
	}
	return false;
}

//std::string GetBuildInParamName(E_BUILD_IN_PARAM param)
//{
//	return BuildInParamNames[param];
//}

//E_SHADER_TYPE GetShaderTypeFromString(string str)
//{
//	return E_SHADER_TYPE();
//}
//
//E_PARAM_TYPE GetParamTypeFromString(string str)
//{
//	return E_PARAM_TYPE();
//}



size_t C_Renderer::AddRenderPass(string _name, size_t _subpass_size)
{
	size_t handle = renderPassesPool.assignData();
	C_RenderPass &render_pass = renderPassesPool.data[handle];
	render_pass.SetSubpassSize(_subpass_size);
	pRenderPasses.insert(make_pair(_name, handle));
	return handle + 1;
}

size_t C_Renderer::GetRenderPass(string _name)
{
	map<string, size_t>::iterator result = pRenderPasses.find(_name);
	if (result != pRenderPasses.end())
	{
		return result->second + 1;
	}
	return 0;
}

void C_Renderer::SetRenderPassView(size_t _render_pass, size_t _view)
{
	if (_render_pass > 0 && _view > 0)
	{
		renderPassesPool.data[_render_pass - 1].pView = _view;
	}
}

void C_Renderer::SetSubpassView(size_t _render_pass, size_t _subpass, size_t _view)
{
	if (_render_pass > 0 && _view > 0)
	{
		renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pView = _view;
	}
}

void C_Renderer::SetSubpassAsFullScreenProcess(size_t _render_pass, size_t _subpass, string _shader_pos_name, string _shader_texcoord_name)
{
	if (_render_pass > 0 && renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline > 0)
	{
		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		if (_shader_pos_name != "" && _shader_texcoord_name != "")
		{
			C_Pipeline &pipeline = pipelinePool.data[subpass.pPipeline - 1];
			subpass.fullScreenPos = pipeline.findParam(_shader_pos_name, E_PARAM_TYPE_ATTVEC4);
			subpass.fullScreenTex = pipeline.findParam(_shader_texcoord_name, E_PARAM_TYPE_ATTVEC2);
		}
		subpass.fillScreenProcess = true;
	}
}

void C_Renderer::SetSubpassFramebufferInput(size_t _render_pass, size_t _subpass, size_t _buffer_id, string _shader_pos_name)
{
	if (_render_pass > 0 && renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline > 0)
	{
		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		C_Pipeline &pipeline = pipelinePool.data[subpass.pPipeline - 1];
		GLint param_pos = pipeline.findParam(_shader_pos_name, E_PARAM_TYPE_UNISAMP2D);
		subpass.framebufferInputs.push_back({ _buffer_id, param_pos, 0 });
	}
}

void C_Renderer::SetSubpassFramebufferOutput(size_t _render_pass, size_t _subpass, size_t _buffer_id, bool _color_or_depth)
{
	if (_render_pass > 0 && renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline > 0)
	{

		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		if (_color_or_depth)
		{
			subpass.colorOutputID = _buffer_id;
		}
		else
		{
			subpass.depthOutputID = _buffer_id;
		}
	}
}

void C_Renderer::BindFramebuffer(size_t _render_pass, size_t _frame_buffer)
{
	if (_render_pass > 0)
	{
		C_RenderPass &render_pass = renderPassesPool.data[_render_pass - 1];
		C_FrameBuffer &frame_buffer = frameBuffersPool.data[_frame_buffer - 1];
		size_t frame_buffer_size = frame_buffer.mTextures.size();
		for (C_RenderPass::S_Subpass &subpass : render_pass.subpasses)
		{
			//inputs
			for (C_RenderPass::S_FramebufferInputMap &map : subpass.framebufferInputs)
			{
				if (map.bufferId >= frame_buffer_size || frame_buffer.mTextures[map.bufferId].type > E_TTYPE_DEPTH)
				{
					debug_func::ReportErr("Framebuffer is not capable with subpass");
				}
				map.bufferTex = frame_buffer.mTextures[map.bufferId].textureID;
			}
			//outputs
			if ((subpass.colorOutputID < UINT_MAX && subpass.colorOutputID > frame_buffer_size)
				|| (subpass.depthOutputID < UINT_MAX && subpass.depthOutputID > frame_buffer_size)
				)
			{
				debug_func::ReportErr("Framebuffer is not capable with subpass");
			}
			if (frame_buffer.mTextures[subpass.colorOutputID].type < E_TYPE_OUTPUT_COLOR || frame_buffer.mTextures[subpass.depthOutputID].type < E_TYPE_OUTPUT_COLOR)
			{
				if (!subpass.frameBufferInitialized)
				{
					glGenFramebuffers(1, &subpass.frameBufferObject);
					subpass.frameBufferInitialized = true;
				}
				if (subpass.colorOutputID < UINT_MAX)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, subpass.frameBufferObject);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, frame_buffer.mTextures[subpass.colorOutputID].textureID, 0);
				}
				if (subpass.depthOutputID < UINT_MAX)
				{
					glBindFramebuffer(GL_FRAMEBUFFER, subpass.frameBufferObject);
					glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, frame_buffer.mTextures[subpass.depthOutputID].textureID, 0);
				}
			}
			else
			{
				if (subpass.frameBufferInitialized)
				{
					glDeleteFramebuffers(1, &subpass.frameBufferObject);
					subpass.frameBufferInitialized = false;
				}
			}

		}
		
		//C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
	}
}

void C_Renderer::SetSubpassOperations(size_t _render_pass, size_t _subpass, std::vector<E_SUBPASS_OPERATION> _ops)
{
	if (_render_pass > 0)
	{
		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		subpass.operations = _ops;
	}
}

void C_Renderer::SetRenderingConfig(std::string config_name)
{
	auto config_itr = pRenderpassConfigs.find(config_name);
	if (config_itr != pRenderpassConfigs.end())
	{
		currentRenderpassConfig = config_itr->second;
	}
}

S_DepthInfo const & C_Renderer::GetDepthInfo() const
{
	return depthInfo;
}

void C_Renderer::Register(C_World &world, std::vector<int> &entityList)
{

	C_ResourceManager &mgr = *C_ResourceManager::GetInstance();
	for (int &id : entityList)
	{
		C_Entity &entity = world.mEntities[id];
		//material
		if ((entity.entityMask & std::bitset<64>(E_CPM_MATERIAL)).any())
		{
				C_MaterialComponent &mat_cmp = world.mMaterials[entity.mComponents[E_CP_MATERIAL]];
				//debug_func::Report("register");
				if (mat_cmp.materialID)
				{
					break;
				}
				mat_cmp.materialID = GetMaterial(mat_cmp.material);
				mat_cmp.materialInstID = AddMaterialInstance(mat_cmp.materialID);
				size_t param_size = mat_cmp.pValues.size();
				for (size_t i = 0; i < param_size; ++i)
				{
					size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, mat_cmp.names[i], mat_cmp.types[i]);
					SetMaterialInstanceParamValue(
						mat_cmp.materialID, 
						mat_cmp.materialInstID, 
						parameter_id, 
						1, 
						GetIdleParam(mat_cmp.types[i],mat_cmp.paramPool, mat_cmp.pValues[i] - 1),
						false);
				}
				mat_cmp.updated = true;
				//mesh
				if((entity.entityMask & std::bitset<64>(E_CPM_MESH)).any())
				{
					C_MeshComponent &mesh_cmp = world.mMeshs[entity.mComponents[E_CP_MESH]];
					const S_MeshResource &mesh = mgr.GetMeshResource(mesh_cmp.mName);
					
					size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_POS), E_PARAM_TYPE_ATTVEC4);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 0, &mesh.mVertexBuffer, false);
					//SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &BufferIDs[0], false);

					parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_TEXCOORD), E_PARAM_TYPE_ATTVEC2);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 0, &mesh.mTextureCoordBuffer, false);
					//SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &BufferIDs[2], false);

					SetMaterialInstanceDrawIndices(mat_cmp.materialID, mat_cmp.materialInstID, &mesh.mFaceBuffer, mesh.mFaceCount * 3);
					//SetMaterialInstanceDrawIndices(mat_cmp.materialID, mat_cmp.materialInstID, &BufferIDs[3], 6);

				}
				//text
				if ((entity.entityMask & std::bitset<64>(E_CPM_TEXT)).any())
				{
					txtSys.Register(world, entity.mComponents[E_CP_TEXT]);
					C_TextComponent &text_cmp = world.mTextComponents[entity.mComponents[E_CP_TEXT]];
					size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_TEX_SCALE), E_PARAM_TYPE_UNIVEC2);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &text_cmp.scale, false);

					parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_TEX_SAMP), E_PARAM_TYPE_UNISAMP2D);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &text_cmp.textureID, false);
				}
				//particle
				if ((entity.entityMask & std::bitset<64>(E_CPM_PARTICLE)).any() && ENABLE_PARTICLES)
				{
					C_ParticleComponent &particle_cmp = world.mParticles[entity.mComponents[E_CP_PARTICLE]];

					size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_INITIAL_LIFETIME), E_PARAM_TYPE_UNIFLOAT);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.initialLifetime, false);

					parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_SIZE), E_PARAM_TYPE_UNIVEC2);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.size, false);
					if (!particle_cmp.runOnGPU)
					{
						glGenBuffers(1, &particle_cmp.positionBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, particle_cmp.positionBuffer);
						glBufferData(GL_ARRAY_BUFFER, particle_cmp.maxNumber * sizeof(vec3), NULL, GL_STREAM_DRAW);

						glGenBuffers(1, &particle_cmp.lifetimeBuffer);
						glBindBuffer(GL_ARRAY_BUFFER, particle_cmp.lifetimeBuffer);
						glBufferData(GL_ARRAY_BUFFER, particle_cmp.maxNumber * sizeof(GLfloat), NULL, GL_STREAM_DRAW);

						parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_POS), E_PARAM_TYPE_ATTVEC3);
						SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.positionBuffer, false);

						parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_LIFETIME), E_PARAM_TYPE_ATTFLOAT);
						SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.lifetimeBuffer, false);

						SetMaterialInstanceInstNum(mat_cmp.materialID, mat_cmp.materialInstID, &particle_cmp.number);
						particles.insert(id);
					}
					else
					{
						gpuParticleSystem.RegisterGpuParticle(world, id, entity.mComponents[E_CP_PARTICLE]);

						parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_POS_DETAILED), E_PARAM_TYPE_ATTVEC3_DETAILED);
						SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.gpuPosInfo, false);

						parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_VEL_DETAILED), E_PARAM_TYPE_ATTVEC3_DETAILED);
						SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.gpuVelInfo, false);

						parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_PARTICLE_LIFETIME_DETAILED), E_PARAM_TYPE_ATTFLOAT_DETAILED);
						SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &particle_cmp.gpuLifetimeInfo, false);

						SetMaterialInstanceInstNum(mat_cmp.materialID, mat_cmp.materialInstID, &particle_cmp.gpuInstNum);
					}
				}
				//transform
				C_Transform &transform_cmp = world.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
				size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_MODEL_MAT), E_PARAM_TYPE_UNIMAT4);

				SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &transform_cmp.TransMatrix[0], false);
				//mat_p = &transform_cmp.TransMatrix;
				//SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &default_mat[0], false);
				//animation	
				if ((entity.entityMask & std::bitset<64>(E_CPM_ANIMATION)).any())
				{
					C_AnimateComponent &anim_comp = world.mAnimations[entity.mComponents[E_CP_ANIMATION]];
					size_t parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_UV_OFFSET), E_PARAM_TYPE_UNIVEC2);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &anim_comp.offset, false);

					parameter_id = GetMaterialParameter(mat_cmp.materialID, GetBuildInParamName(E_BUILDIN_UV_SCALE), E_PARAM_TYPE_UNIVEC2);
					SetMaterialInstanceParamValue(mat_cmp.materialID, mat_cmp.materialInstID, parameter_id, 1, &anim_comp.scale, false);
				}
				materials.insert(id);
		}
		//camera
		if ((entity.entityMask & std::bitset<64>(E_CPM_CAMERA)).any())
		{
			C_Transform &transform_cmp = world.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
			C_CameraComponent &cmp = world.mCameras[entity.mComponents[E_CP_CAMERA]];
			//if (cmp.viewId > 0)
			//{
			//	break;
			//}
			//find view interface
			cmp.viewId = GetView(cmp.viewName);
			//init camera view matrix
			vec4 view_dir = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, 1.0f, 0.0f);
			vec4 view_up = transform_cmp.TransMatrix * vec4(0.0f, 1.0f, 0.0f, 0.0f);
			vec4 view_pos = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f);
			cmp.viewMat = glm::lookAt(vec3(view_pos + view_dir), vec3(view_pos), vec3(view_up));
			//init camera project matrix
			if (cmp.keepScreenSize)
			{
				cmp.widthSize = (float)width;
			}
			cmp.aspect = (float)width / (float)height;
			if (cmp.ortho)
			{
				float half_width = cmp.widthSize * 0.5f;
				float half_height = half_width / cmp.aspect;
				cmp.projectMat = glm::ortho(-half_width, half_width, -half_height, half_height);
			}
			else
			{
				cmp.projectMat = glm::perspective(cmp.fov * onedevPI, cmp.aspect, cmp.nearPlane, 1000.0f);
			}

			SetView(cmp.viewId, &cmp.viewMat[0], &cmp.projectMat[0]);
			cameras.insert(id);
		}
		//widget
		if ((entity.entityMask & std::bitset<64>(E_CPM_WIDGET)).any())
		{
			widgetSys.Register(world, id);
		}

		//collider
		//std::bitset<64> test = entity.entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER);
		if (debugMaterial > 0 && (entity.entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER | E_CPM_BOX_COLLIDER)).any())
		{
			size_t inst = AddMaterialInstance(debugMaterial);
			vec2 collider_size(1.0f, 1.0f);
			vec2 collider_offset(0.0f, 0.0f);
			C_Collider *collider = nullptr;
			if ((entity.entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
			{
				collider = &world.mCircleColliders[entity.mComponents[E_CP_CIRCLE_COLLIDER]];
				circleColliders.insert(make_pair(id, inst));
			}
			else if ((entity.entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
			{
				collider = &world.mPolygonColliders[entity.mComponents[E_CP_BOX_COLLIDER]];
				rectColliders.insert(make_pair(id, inst));
			}
			C_Collider::ShapeType type = collider->mType;
			const S_MeshResource *mesh = nullptr;
			switch (collider->mType)
			{
			case C_Collider::CIRCLE:
			{
				C_CircleCollider *circle = static_cast<C_CircleCollider*>(collider);
				collider_offset = circle->centerOffset;
				collider_size *= circle->mRadius;
				mesh = &mgr.GetMeshResource("circle");
				break;
			}
			case C_Collider::BOX:
			{
				C_Polygon *box = static_cast<C_Polygon*>(collider);
				collider_offset = box->centerOffset;
				collider_size = vec2(box->mWidth * 0.5f, box->mHeight*0.5f);
				mesh = &mgr.GetMeshResource("plane");
				break;
			}
			default:
				break;
			}
			C_Transform &transform_cmp = world.mTransforms[entity.mComponents[E_CP_TRANSFORM]];


			//set transform;
			size_t parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_MODEL_MAT), E_PARAM_TYPE_UNIMAT4);
			SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, &transform_cmp.TransMatrix[0], false);
			//transform_cmp.WorldQuaternion.
			//set relative transform
			mat4 rel_trans = translate(mat4(1.0f), vec3(collider_offset,0.0f)) * scale(mat4(1.0f), vec3(collider_size, 1.0f));
			parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_COLLIDER_REL_MAT), E_PARAM_TYPE_UNIMAT4);
			SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, new mat4(rel_trans), true);

			//set mesh
			parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_POS), E_PARAM_TYPE_ATTVEC4);
			SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 0, &mesh->mVertexBuffer, false);

			parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_TEXCOORD), E_PARAM_TYPE_ATTVEC2);
			SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 0, &mesh->mTextureCoordBuffer, false);

			SetMaterialInstanceDrawIndices(debugMaterial, inst, &mesh->mFaceBuffer, mesh->mFaceCount * 3);

		}
	}

}

void C_Renderer::UpdateMaterials(C_World & worldname)
{
	//update materials
	std::set<int>::iterator mitr = materials.begin();
	std::set<int>::iterator mend = materials.end();
	while (mitr != mend)
	{
		C_Entity &entity = worldname.mEntities[*mitr];
		C_MaterialComponent &mcmp = worldname.mMaterials[entity.mComponents[E_CP_MATERIAL]];
		if (!mcmp.updated) {
			size_t param_size = mcmp.pValues.size();
			for (size_t i = 0; i < param_size; ++i)
			{
				size_t parameter_id = GetMaterialParameter(mcmp.materialID, mcmp.names[i], mcmp.types[i]);
				SetMaterialInstanceParamValue(
					mcmp.materialID,
					mcmp.materialInstID,
					parameter_id,
					1,
					GetIdleParam(mcmp.types[i], mcmp.paramPool, mcmp.pValues[i] - 1),
					false);
			}
			mcmp.updated = true;
		}
		++mitr;
	}
}

void C_Renderer::InitGPUParticleSystem()
{

}

void C_Renderer::UpdateParticles(C_World & worldname, float _delta_time)
{
	std::set<int>::iterator pitr = particles.begin();
	std::set<int>::iterator pend = particles.end();
	while (pitr != pend)
	{
		C_Entity &entity = worldname.mEntities[*pitr];
		C_ParticleComponent &cmp = worldname.mParticles[entity.mComponents[E_CP_PARTICLE]];

		if (cmp.number > 0) {
			glBindBuffer(GL_ARRAY_BUFFER, cmp.positionBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, cmp.number * sizeof(vec3), &cmp.positions[0]);

			glBindBuffer(GL_ARRAY_BUFFER, cmp.lifetimeBuffer);
			glBufferSubData(GL_ARRAY_BUFFER, 0, cmp.number * sizeof(GLfloat), &cmp.lifetime[0]);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}

		//if (cmp.play || cmp.number > 0)
		//{
		//	C_Transform &transform_cmp = worldname.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
		//	vec3 start_pos = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f);
		//	//remove died particles
		//	size_t current_id = 0;
		//	while (current_id < cmp.number)
		//	{
		//		cmp.lifetime[current_id] -= _delta_time;
		//		if (cmp.lifetime[current_id] <= 0.0f)//invalidate
		//		{
		//			cmp.positions[current_id] = cmp.positions[cmp.number - 1];
		//			cmp.velocities[current_id] = cmp.velocities[cmp.number - 1];
		//			cmp.lifetime[current_id] = cmp.lifetime[cmp.number - 1];
		//			--cmp.number;
		//		}
		//		else
		//		{
		//			++current_id;
		//		}
		//	}
		//	//update alive particles
		//	for (size_t i = 0; i < cmp.number; ++i)
		//	{
		//		cmp.velocities[i] += cmp.accel * _delta_time;
		//		cmp.positions[i] += cmp.velocities[i] * _delta_time;
		//		//float speed = glm::length(cmp.velocities[current_id]);
		//	}
		//	cmp.currentInterval -= _delta_time;
		//	if (cmp.currentInterval <= 0.0f)
		//	{
		//		//spawn
		//		size_t spawn_number = min(cmp.spawnNumber, cmp.maxNumber - cmp.number);
		//		for (size_t i = 0; i < spawn_number; ++i)
		//		{
		//			size_t true_id = cmp.number + i;
		//			cmp.lifetime[true_id] = cmp.initialLifetime;
		//			cmp.positions[true_id] = start_pos;
		//			cmp.velocities[true_id] = cmp.initialSpeed * (rotate(mat4(1.0f), i * 0.1f, vec3(0.0f, 0.0f, 1.0f)) * vec4(1.0f, 0.0f, 0.0f, 0.0f));
		//		}
		//		cmp.number += spawn_number;
		//		cmp.currentInterval = 0.0f;
		//		if (cmp.loop)
		//		{
		//			cmp.currentInterval = cmp.interval;
		//		}
		//	}
		//	glBindBuffer(GL_ARRAY_BUFFER, cmp.positionBuffer);
		//	glBufferData(GL_ARRAY_BUFFER, cmp.maxNumber * sizeof(vec3), NULL, GL_STREAM_DRAW);
		//	if (cmp.number > 0) {
		//		glBufferSubData(GL_ARRAY_BUFFER, 0, cmp.number * sizeof(vec3), &cmp.positions[0]);
		//	}
		//	glBindBuffer(GL_ARRAY_BUFFER, 0);
		//	if (!cmp.loop)
		//	{
		//		cmp.play = false;
		//	}
		//}
		++pitr;
	}
}

bool C_Renderer::ShouldUpdateSize()
{
	return needUpdateSize;
}

void C_Renderer::Resize(size_t _new_width, size_t _new_height)
{
	width = _new_width;
	height = _new_height;

	for (auto &id : pFrameBuffers)
	{
		RefreshFrameBufferStruct(id.second + 1);
	}
	needUpdateSize = true;
}

bool C_Renderer::IsInView(vec3 _position, std::string _view_name)
{
	auto itr = pViews.find(_view_name);
	if (itr == pViews.end())
	{
		return false;
	}
	vec4 result = (*reinterpret_cast<mat4 const *>(viewPool.data[itr->second].pProjectMatrix)) * (*reinterpret_cast<mat4 const *>(viewPool.data[itr->second].pProjectMatrix)) * vec4(_position, 1.0f);
	result /= result.w;
	return (fabsf(result.x) <= 1.0f && fabsf(result.y) <= 1.0f);
}

void C_Renderer::Update(C_World & worldname, float deltaTime)
{
	if (outputWindow->Activated) 
	{
		//update camera
		std::set<int>::iterator itr = cameras.begin();
		std::set<int>::iterator end = cameras.end();
		while (itr != end)
		{
			C_Entity &entity = worldname.mEntities[*itr];
			C_CameraComponent &cmp = worldname.mCameras[entity.mComponents[E_CP_CAMERA]];
			if (cmp.enabled)
			{
				//find view interface
				cmp.viewId = GetView(cmp.viewName);
				C_Transform &transform_cmp = worldname.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
				//init camera view matrix
				vec4 view_dir = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, -1.0f, 0.0f);
				vec4 view_up = transform_cmp.TransMatrix * vec4(0.0f, 1.0f, 0.0f, 0.0f);
				vec4 view_pos = transform_cmp.TransMatrix * vec4(0.0f, 0.0f, 0.0f, 1.0f);
				cmp.viewMat = glm::lookAt(vec3(view_pos), vec3(view_pos + view_dir), vec3(view_up));
				//cmp.viewMat = mat4(1.0f);
				//init camera project matrix
				cmp.aspect = (float)width / (float)height;
				if (cmp.ortho)
				{
					if (cmp.keepScreenSize)
					{
						cmp.widthSize = (float)width;
					}
					float half_width = cmp.widthSize * 0.5f;
					float half_height = half_width / cmp.aspect;
					cmp.projectMat = glm::orthoRH_NO(-half_width, half_width, -half_height, half_height, -100.0f, 100.0f);
					//cmp.projectMat = mat4(1.0f);
				}
				else
				{
					cmp.projectMat = glm::perspective(cmp.fov * onedevPI, cmp.aspect, cmp.nearPlane, 1000.0f);
				}
			}
			++itr;
		}
		//debug_func::Report(std::to_string(deltaTime));
#ifdef _DEBUG
		static float remaintime = 0.0f;
		remaintime += deltaTime;
		if (remaintime > 0.5f)
		{
			SetWindowTextA(outputWindow->GetWin32Handle(), ("Dark Flare (frame rate: " + std::to_string(1.0 / deltaTime) + "fps)").c_str());
			remaintime = 0.0f;
		}
#endif
		UpdateDebuglines(worldname);
		widgetSys.Update(worldname, needUpdateSize);
		txtSys.Update(worldname);
		UpdateParticles(worldname, 0.016);
		UpdateMaterials(worldname);
		//draw
		Render();
		gpuParticleSystem.Update(worldname, deltaTime);
		needUpdateSize = false;
	}
}

void C_Renderer::Unregister(C_World & worldname, std::vector<int>& entityList)
{

	debug_func::Report("UnRegister");
	C_ResourceManager &mgr = *C_ResourceManager::GetInstance();
	for (int &id : entityList)
	{
		C_Entity &entity = worldname.mEntities[id];
		//material
		if ((entity.entityMask & std::bitset<64>(E_CPM_MATERIAL)).any())
		{
			C_MaterialComponent &mat_cmp = worldname.mMaterials[entity.mComponents[E_CP_MATERIAL]];
			if (mat_cmp.materialInstID > 0)
			{
				DestroyMaterialInstance(mat_cmp.materialID, mat_cmp.materialInstID);
				mat_cmp.materialInstID = 0;
				mat_cmp.materialID = 0;
			}
			materials.erase(id);
			//text
			if ((entity.entityMask & std::bitset<64>(E_CPM_TEXT)).any())
			{
				txtSys.Unregister(worldname, entity.mComponents[E_CP_TEXT]);
			}
		}
		//camera
		if ((entity.entityMask & std::bitset<64>(E_CPM_CAMERA)).any())
		{
			cameras.erase(id);
		}
		//widget
		if ((entity.entityMask & std::bitset<64>(E_CPM_WIDGET)).any())
		{
			widgetSys.Unregister(worldname, id);
		}
		//particle
		if ((entity.entityMask & std::bitset<64>(E_CPM_PARTICLE)).any() && ENABLE_PARTICLES)
		{
			C_ParticleComponent &particle_cmp = worldname.mParticles[entity.mComponents[E_CP_PARTICLE]];
			if (!particle_cmp.runOnGPU)
			{
				glDeleteBuffers(1, &particle_cmp.positionBuffer);
				glDeleteBuffers(1, &particle_cmp.lifetimeBuffer);
				particles.erase(id);
			}
			else
			{
				//std::cout << entity.mComponents[E_CP_PARTICLE] << " ";
				gpuParticleSystem.UnregisterGpuParticle(worldname, id, entity.mComponents[E_CP_PARTICLE]);
			}
		}
		//collider
		if ((entity.entityMask & std::bitset<64>(E_CPM_CIRCLE_COLLIDER)).any())
		{
			std::map<int,size_t>::iterator result = circleColliders.find(id);
			if (result != circleColliders.end())
			{
				DestroyMaterialInstance(debugMaterial, result->second);
				circleColliders.erase(result);
			}
		}
		if ((entity.entityMask & std::bitset<64>(E_CPM_BOX_COLLIDER)).any())
		{
			std::map<int, size_t>::iterator result = rectColliders.find(id);
			if (result != rectColliders.end())
			{
				DestroyMaterialInstance(debugMaterial, result->second);
				rectColliders.erase(result);
			}
		}
	}
	//std::cout << std::endl;
	//for (int &i : entityList)
	//{
	//	cameras.erase(i);
	//}
}

void C_Renderer::UpdateDebuglines(C_World &worldname)
{
	for (auto &itr : circleColliders)
	{
		C_Entity &entity = worldname.mEntities[itr.first];
		C_Transform &transform_cmp = worldname.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
		C_CircleCollider &collider = worldname.mCircleColliders[entity.mComponents[E_CP_CIRCLE_COLLIDER]];
		vec2 collider_offset = collider.centerOffset;
		vec2 collider_size(collider.mRadius, collider.mRadius);
		size_t inst = itr.second;
		//set transform;
		size_t parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_MODEL_MAT), E_PARAM_TYPE_UNIMAT4);
		SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, &transform_cmp.TransMatrix[0], false);
		//transform_cmp.WorldQuaternion.
		//set relative transform
		mat4 rel_trans = translate(mat4(1.0f), vec3(collider_offset, 0.0f)) * scale(mat4(1.0f), vec3(collider_size, 1.0f));
		parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_COLLIDER_REL_MAT), E_PARAM_TYPE_UNIMAT4);
		SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, new mat4(rel_trans), true);
	}
	for (auto &itr : rectColliders)
	{
		C_Entity &entity = worldname.mEntities[itr.first];
		C_Transform &transform_cmp = worldname.mTransforms[entity.mComponents[E_CP_TRANSFORM]];
		C_Polygon &collider = worldname.mPolygonColliders[entity.mComponents[E_CP_BOX_COLLIDER]];
		vec2 collider_offset = collider.centerOffset;
		vec2 collider_size(collider.mWidth * 0.5f, collider.mHeight*0.5f);
		size_t inst = itr.second;
		//set transform;
		size_t parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_MODEL_MAT), E_PARAM_TYPE_UNIMAT4);
		SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, &transform_cmp.TransMatrix[0], false);
		//transform_cmp.WorldQuaternion.
		//set relative transform
		mat4 rel_trans = translate(mat4(1.0f), vec3(collider_offset, 0.0f)) * QuaternionToRotationMatrix(transform_cmp.WorldQuaternion) * scale(mat4(1.0f), vec3(collider_size, 1.0f));
		parameter_id = GetMaterialParameter(debugMaterial, GetBuildInParamName(E_BUILDIN_COLLIDER_REL_MAT), E_PARAM_TYPE_UNIMAT4);
		SetMaterialInstanceParamValue(debugMaterial, inst, parameter_id, 1, new mat4(rel_trans), true);
	}

}

void C_Renderer::SetupDefaultRenderer()
{
	SetupRendererDatas("Renderer.json");
}

namespace
{
	std::map<std::string, E_PARAM_TYPE> param_type_map = 
	{
		std::pair<std::string, E_PARAM_TYPE>("uint",E_PARAM_TYPE_UNIINT),
		std::pair<std::string, E_PARAM_TYPE>("ufloat",E_PARAM_TYPE_UNIFLOAT),
		std::pair<std::string, E_PARAM_TYPE>("uvec2",E_PARAM_TYPE_UNIVEC2),
		std::pair<std::string, E_PARAM_TYPE>("uvec3",E_PARAM_TYPE_UNIVEC3),
		std::pair<std::string, E_PARAM_TYPE>("uvec4",E_PARAM_TYPE_UNIVEC4),
		std::pair<std::string, E_PARAM_TYPE>("umat4",E_PARAM_TYPE_UNIMAT4),
		std::pair<std::string, E_PARAM_TYPE>("usampler2d",E_PARAM_TYPE_UNISAMP2D),
		std::pair<std::string, E_PARAM_TYPE>("afloat",E_PARAM_TYPE_ATTFLOAT),
		std::pair<std::string, E_PARAM_TYPE>("avec2",E_PARAM_TYPE_ATTVEC2),
		std::pair<std::string, E_PARAM_TYPE>("avec3",E_PARAM_TYPE_ATTVEC3),
		std::pair<std::string, E_PARAM_TYPE>("avec4",E_PARAM_TYPE_ATTVEC4),
		std::pair<std::string, E_PARAM_TYPE>("afloat_detail",E_PARAM_TYPE_ATTFLOAT_DETAILED),
		std::pair<std::string, E_PARAM_TYPE>("avec3_detail",E_PARAM_TYPE_ATTVEC3_DETAILED),
		std::pair<std::string, E_PARAM_TYPE>("avec4_detail",E_PARAM_TYPE_ATTVEC4_DETAILED),
	};

	std::map<std::string, E_TEXTURE_BUFFER_TYPE> texbuffer_type_map = 
	{
		std::pair<std::string, E_TEXTURE_BUFFER_TYPE>("RGBA", E_TTYPE_RGBA),
		std::pair<std::string, E_TEXTURE_BUFFER_TYPE>("RGBA", E_TTYPE_RGB),
		std::pair<std::string, E_TEXTURE_BUFFER_TYPE>("DEPTH", E_TTYPE_DEPTH),
		std::pair<std::string, E_TEXTURE_BUFFER_TYPE>("ORGBA", E_TYPE_OUTPUT_COLOR),
		std::pair<std::string, E_TEXTURE_BUFFER_TYPE>("ODEPTH", E_TYPE_OUTPUT_DEPTH),
	};

	std::map<std::string, E_SUBPASS_OPERATION> subpass_op_map =
	{
		std::pair<std::string, E_SUBPASS_OPERATION>("CLEAR_COLOR", E_OP_CLEAR_COLOR),
		std::pair<std::string, E_SUBPASS_OPERATION>("CLEAR_DEPTH", E_OP_CLEAR_DEPTH),
		std::pair<std::string, E_SUBPASS_OPERATION>("CLEAR_COLOR_DEPTH", E_OP_CLEAR_COLOR_DEPTH),
		std::pair<std::string, E_SUBPASS_OPERATION>("DISABLE_DEPTH", E_OP_DISABLE_DEPTH),
		std::pair<std::string, E_SUBPASS_OPERATION>("ENABLE_DEPTH", E_OP_ENABLE_DEPTH),
		std::pair<std::string, E_SUBPASS_OPERATION>("BLEND_ACC", E_OP_BLEND_ACC),
		std::pair<std::string, E_SUBPASS_OPERATION>("BLEND_ITERP", E_OP_BLEND_ITERP),
		std::pair<std::string, E_SUBPASS_OPERATION>("POLYGON_FILL", E_OP_POLYGON_MODE_FILL),
		std::pair<std::string, E_SUBPASS_OPERATION>("POLYGON_LINE", E_OP_POLYGON_MODE_LINE),
	};
}

void C_Renderer::SetupRendererDatas(std::string path)
{
	C_ResourceManager *rsc_mgr = C_ResourceManager::GetInstance();
	size_t view_handle = 0,
		pipeline_handle = 0,
		mat_handle = 0,
		param_handle = 0,
		frame_buffer_handle = 0,
		renderpass_handle = 0;
	S_JsonValue data = json_func::LoadJsonData(file_func::GetPath(GRAPHICS_FOLDER + path)).ExpandSrc();
	//load pipelines
	if (!data["pipelines"].isNull())
	{
		std::vector<std::string> names = data["pipelines"].GetMemberNames();
		for (std::string &pip_name : names)
		{
			std::string vert, frag;
			size_t size = data["pipelines"][pip_name].size();
			for (size_t i = 0; i < size; ++i)
			{
				std::string str = data["pipelines"][pip_name][i].asString();
				std::string suffix = file_func::GetSuffix(str);
				if (suffix == ".vert")
				{
					vert = str;
				}
				else if (suffix == ".frag")
				{
					frag = str;
				}
			}
			pipeline_handle = AddPipeline(pip_name, vert, frag);
		}
	}
	//materials
	if (!data["materials"].isNull())
	{
		std::vector<std::string> names = data["materials"].GetMemberNames();
		for (std::string &mat_name : names)
		{
			mat_handle = AddMaterial(mat_name);
			S_JsonValue mat_data = data["materials"][mat_name];
			std::vector<std::string> paramtype_names = mat_data.GetMemberNames();
			for (std::string &param_type : paramtype_names)
			{
				E_PARAM_TYPE type = param_type_map[param_type];
				std::vector<std::string> param_names = mat_data[param_type].GetMemberNames();
				for (std::string &param : param_names)
				{
					param_handle = AddMaterialParameter(mat_handle, param, type);
					Json::Value pval = mat_data[param_type][param].GetJsonValue();
					if (type < E_PARAM_TYPE_ATTVEC2 && !(pval.isArray() && mat_data[param_type][param].size() == 0))//set default value
					{
						switch (type)
						{
						case E_PARAM_TYPE_UNIINT:
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, new GLint(mat_data[param_type][param].asInt()), true);
							break;
						case E_PARAM_TYPE_UNIFLOAT:
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, new GLfloat(mat_data[param_type][param].asFloat()), true);
							break;
						case E_PARAM_TYPE_UNIVEC2:
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, new vec2(mat_data[param_type][param].asVec2()), true);
							break;
						case E_PARAM_TYPE_UNIVEC3:
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, new vec3(mat_data[param_type][param].asVec3()), true);
							break;
						case E_PARAM_TYPE_UNIVEC4:
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, new vec4(mat_data[param_type][param].asVec4()), true);
							break;
						case E_PARAM_TYPE_UNISAMP2D:
							S_TextureResource const &tex = rsc_mgr->GetTexture(mat_data[param_type][param].asString().c_str());
							SetMaterialParameterDefaultValue(mat_handle, param_handle, 1, &tex.texture, false);
							break;
						}
					}
				}
				
			}
		}
	}
	//frame buffers
	if (!data["framebuffers"].isNull())
	{
		std::vector<std::string> names = data["framebuffers"].GetMemberNames();
		for (std::string &fbname : names)
		{
			frame_buffer_handle = AddFrameBuffer(fbname);
			size_t size = data["framebuffers"][fbname].size();
			std::vector<E_TEXTURE_BUFFER_TYPE> fb_structure(size);
			for (size_t i = 0; i < size; ++i)
			{
				fb_structure[i] = texbuffer_type_map[data["framebuffers"][fbname][i].asString()];
			}
			SetFrameBufferStruct(frame_buffer_handle, fb_structure);
		}
	}
	//renderpasses
	if (!data["renderpasses"].isNull())
	{
		std::vector<std::string> names = data["renderpasses"].GetMemberNames();
		for (std::string &passname : names)
		{
			frame_buffer_handle = 0;
			size_t pView = 0;
			S_JsonValue renderpass_data = data["renderpasses"][passname];
			std::vector<std::string> section_names = renderpass_data.GetMemberNames();
			for (std::string &section_name : section_names)
			{
				if (section_name == "subpasses")
				{
					size_t subpass_sz = renderpass_data[section_name].size();
					//create render pass and insert into subpasses
					renderpass_handle = AddRenderPass(passname, subpass_sz);
					for (size_t subpass_id = 0; subpass_id < subpass_sz; ++subpass_id)
					{
						S_JsonValue sp_data = renderpass_data[section_name][subpass_id];
						//pipeline
						if (!sp_data["pipeline"].isNull())
						{
							pipeline_handle = GetPipeline(sp_data["pipeline"].asString());
							SetSubpassPipeline(renderpass_handle, subpass_id, pipeline_handle);
						}
						//others
						std::vector<std::string> sp_section_names = sp_data.GetMemberNames();
						for (std::string &spsec : sp_section_names)
						{
							
							if (spsec == "operations")//operations
							{
								size_t op_size = sp_data[spsec].size();
								std::vector<E_SUBPASS_OPERATION> operations(op_size);
								for (size_t op_id = 0; op_id < op_size; ++op_id)
								{
									operations[op_id] = subpass_op_map[sp_data[spsec][op_id].asString()];
								}
								SetSubpassOperations(renderpass_handle, subpass_id, operations);
							}
							else if (spsec == "viewbind")//view pos
							{
								BindSubpassPipelineViewPos(renderpass_handle, subpass_id, sp_data[spsec][0].asString(), sp_data[spsec][1].asString());
							}
							else if (spsec == "full_screen")//full screen
							{
								SetSubpassAsFullScreenProcess(renderpass_handle, subpass_id, sp_data[spsec][0].asString(), sp_data[spsec][1].asString());
							}
							else if (spsec == "color_output")//color output
							{
								SetSubpassFramebufferOutput(renderpass_handle, subpass_id, sp_data[spsec].asInt(), true);
								
							}
							else if (spsec == "depth_output")//depth output
							{
								SetSubpassFramebufferOutput(renderpass_handle, subpass_id, sp_data[spsec].asInt(), false);
							}
							else if (spsec == "inputs")
							{
								std::vector<std::string> inputnames = sp_data[spsec].GetMemberNames();
								for (std::string &input : inputnames)
								{
									SetSubpassFramebufferInput(renderpass_handle, subpass_id, sp_data[spsec][input].asInt(), input);
								}
							}
							else if (spsec == "materialbinds")
							{
								std::vector<std::string> names = sp_data[spsec].GetMemberNames();
								for (std::string &mat_name : names)
								{
									mat_handle = GetMaterial(mat_name);
									S_JsonValue mat_data = sp_data[spsec][mat_name];
									std::vector<std::string> paramtype_names = mat_data.GetMemberNames();
									for (std::string &param_type : paramtype_names)
									{
										E_PARAM_TYPE type = param_type_map[param_type];
										std::vector<std::string> param_names = mat_data[param_type].GetMemberNames();
										for (std::string &param : param_names)
										{
											BindSubpassMaterialVal(renderpass_handle, subpass_id, mat_handle, type, param, mat_data[param_type][param].asString());
										}
									}
								}
							}
						}
					}
				}
				else if (section_name == "view")
				{
					pView = GetView(renderpass_data[section_name].asString());
				}
				else if (section_name == "default_framebuffer")
				{
					frame_buffer_handle = GetFrameBuffer(renderpass_data[section_name].asString());
				}
			}
			//bind view & frame buffer
			if (pView > 0)
			{
				SetRenderPassView(renderpass_handle, pView);
			}
			if (frame_buffer_handle > 0)
			{
				BindFramebuffer(renderpass_handle, frame_buffer_handle);
			}
		}
	}

	std::string default_renderpass_config_name = "";
	if (!data["defaultRenderpassConfig"].isNull())
	{
		default_renderpass_config_name = data["defaultRenderpassConfig"].asString();
	}
	else
	{
		debug_func::ReportErr("no default renderpass config!");
	}

	if (!data["renderpassConfigs"].isNull())
	{
		std::vector<std::string> config_names = data["renderpassConfigs"].GetMemberNames();
		for (std::string &config_name : config_names)
		{
			size_t size = data["renderpassConfigs"][config_name].size();
			std::vector<size_t> contents;
			for (size_t i = 0; i < size; ++i)
			{
				std::string pass_name = data["renderpassConfigs"][config_name][i].asString();
				contents.push_back(GetRenderPass(pass_name));
				//auto result_itr = pRenderPasses.find(pass_name);
				//if (result_itr != pRenderPasses.end())
				//{
				//	contents.push_back(result_itr->second);
				//}
			}
			size_t id = renderpassConfigs.size();
			renderpassConfigs.push_back(contents);
			pRenderpassConfigs.insert(make_pair(config_name, id));
			if (default_renderpass_config_name == config_name)
			{
				currentRenderpassConfig = id;
			}
		}
	}
	if (!data["depthconfig"].isNull())
	{
		std::vector<std::string> section_names = data["depthconfig"].GetMemberNames();
		for (std::string &section_name : section_names)
		{
			if (section_name == "view")
			{
				view_handle = GetView(data["depthconfig"][section_name].asString());
				if (view_handle > 0)
				{
					depthInfo.view = &viewPool.data[view_handle - 1];
				}
				
			}
			else if (section_name == "framebuffer")
			{
				std::vector<std::string> buffer_names = data["depthconfig"][section_name].GetMemberNames();
				for (auto &buffer_name : buffer_names)
				{
					frame_buffer_handle = GetFrameBuffer(buffer_name);
					size_t tex_id = data["depthconfig"][section_name][buffer_name].asInt();
					depthInfo.depthBuffer = frameBuffersPool.data[frame_buffer_handle - 1].mTextures[tex_id].textureID;
				}
			}
		}
	}
}

void C_Renderer::SetSubpassPipeline(size_t _render_pass, size_t _subpass, size_t _pipeline)
{
	if (_render_pass > 0 && _pipeline > 0)
	{
		renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline = _pipeline;
	}
}

void C_Renderer::BindSubpassMaterialVal(size_t _render_pass, size_t _subpass, size_t _material, 
	E_PARAM_TYPE _param_type, string mat_param_name, string shader_param_name)
{
	if (_render_pass > 0 && _material > 0 && renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline > 0)
	{
		
		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		//size_t _pipeline_id = subpass.pPipeline;
		C_Material &material = materialsPool.data[_material - 1];
		C_Pipeline &pipeline = pipelinePool.data[subpass.pPipeline - 1];
		GLint pipline_pos = pipeline.findParam(shader_param_name, _param_type);
		size_t mat_pos = material.GetParameterData(_param_type, mat_param_name);
		subpass.AddParamPair(_material, pipline_pos, mat_pos);
	}
}

void C_Renderer::BindSubpassPipelineViewPos(size_t _render_pass, size_t _subpass, string _shader_view_name, string _shader_proj_name)
{
	if (_render_pass > 0 && renderPassesPool.data[_render_pass - 1].subpasses[_subpass].pPipeline > 0)
	{
		C_RenderPass::S_Subpass &subpass = renderPassesPool.data[_render_pass - 1].subpasses[_subpass];
		C_Pipeline &pipeline = pipelinePool.data[subpass.pPipeline - 1];
		subpass.viewPositions.viewPos = pipeline.findParam(_shader_view_name, E_PARAM_TYPE_UNIMAT4);
		subpass.viewPositions.projPos = pipeline.findParam(_shader_proj_name, E_PARAM_TYPE_UNIMAT4);
		subpass.viewPositions.binded = true;
	}
}

void C_Renderer::ExecuteRenderPass(size_t _render_pass)
{
	C_RenderPass &render_pass = renderPassesPool.data[_render_pass - 1];
	size_t sub_pass_size = render_pass.subpasses.size();
	//subpass loop
	for (size_t spit = 0; spit < sub_pass_size; ++spit)
	{
		C_RenderPass::S_Subpass &sub_pass = render_pass.subpasses[spit];
		pipelinePool.data[sub_pass.pPipeline - 1].bindPipeline();
		//bind input frame buffers
		textBindPoint = 0;
		for (auto &map : sub_pass.framebufferInputs)
		{
			ParamSetFunctions[E_PARAM_TYPE_UNISAMP2D](map.inputPos, 1, &map.bufferTex);
		}
		//bind output frame buffers
		if (sub_pass.frameBufferInitialized)
		{
			glBindFramebuffer(GL_FRAMEBUFFER, sub_pass.frameBufferObject);
		}
		else
		{
			glBindFramebuffer(GL_FRAMEBUFFER, 0);
		}
		//initial configurations
		//PassOperationClearColorDepth();
		for (E_SUBPASS_OPERATION &op : sub_pass.operations)
		{
			PassOperations[op]();
		}
		//set view parameters
		if (sub_pass.viewPositions.binded)
		{
			//if (sub_pass.fillScreenProcess)//if subpass is a fill screen process, view and proj will be set to I
			//{

			//}
			if (sub_pass.pView != 0)
			{
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.viewPos, 1, viewPool.data[sub_pass.pView - 1].pViewMatrix);
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.projPos, 1, viewPool.data[sub_pass.pView - 1].pProjectMatrix);
			}
			else if (render_pass.pView != 0)
			{
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.viewPos, 1, viewPool.data[render_pass.pView - 1].pViewMatrix);
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.projPos, 1, viewPool.data[render_pass.pView - 1].pProjectMatrix);
			}
			else
			{
				mat4 Imat(1.0f);
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.viewPos, 1, &Imat[0]);
				ParamSetFunctions[E_PARAM_TYPE_UNIMAT4](sub_pass.viewPositions.projPos, 1, &Imat[0]);
			}
		}
		uint16_t material_text_point_offset = textBindPoint;
		//material loop
		for (C_RenderPass::S_PassMatMap &mapping : sub_pass.materialMappings)
		{
			C_Material &material = materialsPool.data[mapping.pMaterial - 1];
			//initialization loop
			textBindPoint = material_text_point_offset;
			for (std::pair<GLint, size_t> &itpair : mapping.paramPairs)
			{
				size_t param_id = itpair.second - 1;
				uint64_t type = material.parameterInfos[param_id].type;
				S_Param &param_data = material.parameters.data[itpair.second];
				//if (param_data.idleParamHandle > 0)
				//{
				//	ParamSetFunctions[type](itpair.first,1, IdleParamGetFunctions[type](material.paramPool, param_data.idleParamHandle));
				//}
				if (param_data.pData != nullptr)
				{
					ParamSetFunctions[type](itpair.first, param_data.size, param_data.pData);
				}
			}
			if (!sub_pass.fillScreenProcess || true)//if this pass is not a processing pass, draw material related contents
			{
				//initial draw if any
				if (material.parameters.data[0].pData != nullptr)
				{
					glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(material.parameters.data[0].pData));
					glDrawElements(GL_TRIANGLES, material.parameters.data[0].size, GL_UNSIGNED_INT, 0);
				}
				//instance loop
				size_t last_inst = 0;
				material.StartInstance();
				while (!material.IsInstanceFinished())
				{
					textBindPoint = material_text_point_offset;
					S_MaterialInst &current = material.instancePool.data[material.pCurrent - 1];
					std::vector<size_t> &parameter_list = current.parameter_handles;
					for (std::pair<GLint, size_t> &itpair : mapping.paramPairs)
					{
						size_t param_id = itpair.second - 1;
						uint64_t type = material.parameterInfos[param_id].type;
						size_t param_inst_id = current.parameter_handles[itpair.second];
						if (last_inst == 0 || param_inst_id != material.instancePool.data[last_inst - 1].parameter_handles[itpair.second])
						{
							S_Param &param_data = material.parameters.data[param_inst_id];
							if (param_data.pData != nullptr)
							{
								ParamSetFunctions[type](itpair.first, param_data.size, param_data.pData);
							}
						}
					}
					//draw if any
					size_t inst_index_id = current.parameter_handles[0];
					if (material.parameters.data[inst_index_id].pData != nullptr && !sub_pass.fillScreenProcess)
					{
						glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *reinterpret_cast<const GLuint*>(material.parameters.data[inst_index_id].pData));
						if (current.instNum != nullptr)
						{
							if (*(current.instNum) > 1)
							{
								glDrawElementsInstanced(GL_TRIANGLES, material.parameters.data[inst_index_id].size, GL_UNSIGNED_INT, 0, *(current.instNum));
							}

						}
						else
						{
							glDrawElements(GL_TRIANGLES, material.parameters.data[inst_index_id].size, GL_UNSIGNED_INT, 0);
						}
					}
					last_inst = material.pCurrent;
					material.NextInstance();
				}
			}
		}
		if (sub_pass.fillScreenProcess)//if current subpass is a fill screen process, draw a fill-screen rectangle
		{
			ParamSetFunctions[E_PARAM_TYPE_ATTVEC4](sub_pass.fullScreenPos, 0, &BufferIDs[0]);
			ParamSetFunctions[E_PARAM_TYPE_ATTVEC2](sub_pass.fullScreenTex, 0, &BufferIDs[2]);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, BufferIDs[3]);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
	}
}

void C_Renderer::Render()
{
	glViewport(0, 0, static_cast<GLsizei>(outputWindow->width), static_cast<GLsizei>(outputWindow->height));
	std::vector<size_t> config = renderpassConfigs[currentRenderpassConfig];
	for (size_t &itr : config)
	{
		ExecuteRenderPass(itr);
	}
	SwapBuffers(win32Device);
}


