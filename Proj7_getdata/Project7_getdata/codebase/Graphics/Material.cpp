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
#include "Graphics/Material.h"

C_Material::C_Material() : indexId(0), pParameterDatas(E_PARAM_TYPE_MAX), parameters(10), instancePool(10), pHead(0), pTail(0), pCurrent(0)
{
	//assign default data for index;
	size_t default_handle = parameters.assignData();
	if (default_handle != indexId)
	{
		debug_func::ReportErr("Invalid default index data id, may caused by changing material data name after instantiation");
	}
}

size_t C_Material::AddParameterData(E_PARAM_TYPE _param_type, std::string _name)
{
	parameterInfos.push_back({ _name, static_cast<uint64_t>(_param_type) });
	size_t id = parameterInfos.size() - 1;
	pParameterDatas[_param_type].insert(std::make_pair(_name, id));

	//add default param
	size_t default_handle = parameters.assignData();
	if (default_handle != id + 1) // index holds 0, so the default id for parameters will add 1
	{
		debug_func::ReportErr("Invalid default param data id, may caused by changing material data name after instantiation");
	}

	return id + 1;
}

size_t C_Material::GetParameterData(E_PARAM_TYPE _param_type, std::string _name)
{
	std::map<std::string, size_t>::iterator result = pParameterDatas[_param_type].find(_name);
	if (result != pParameterDatas[_param_type].end())
	{
		return result->second + 1;
	}
	return 0;
}

void C_Material::SetParameterDefaultValue(size_t parameter_data_handle, uint32_t _size, void const * _p_data, bool _self_manage)
{
	//size_t id = parameter_data_handle - 1;
	//size_t param_pool_handle = 0;
	//if (parameters.data[parameter_data_handle].idleParamHandle > 0)
	//{
	//	FreeIdleParameter(parameterInfos[parameter_data_handle - 1].type, parameters.data[parameter_data_handle].idleParamHandle);
	//}
	//if (_idle)
	//{
	//	param_pool_handle = AssignIdleParameter(parameterInfos[parameter_data_handle - 1].type);
	//	SetIdleParameter(parameterInfos[parameter_data_handle - 1].type, param_pool_handle, _p_data);
	//}
	parameters.data[parameter_data_handle].SetParam(_size, _p_data, _self_manage);
}

size_t C_Material::AddMaterialInstance()
{
	size_t size = parameterInfos.size();
	size_t id = instancePool.assignData();
	//initialize instance as default value;
	S_MaterialInst *new_inst = &instancePool.data[id];
	new_inst->parameter_handles.resize(size + 1);
	for (size_t val = 0; val < size + 1; ++val)
	{
		new_inst->parameter_handles[val] = val;
	}
	size_t handle = id + 1;
	/// add to front
	//new_inst->pFormer = 0;
	//new_inst->pNext = pHead;
	//if (pHead > 0)
	//{
	//	instancePool.data[pHead - 1].pFormer = handle;
	//}
	//pHead = handle;
	//if (pTail == 0)
	//{
	//	pTail = handle;
	//}
	/// 
	if (pHead == 0)
	{
		pHead = handle;
		new_inst->pFormer = 0;
	}
	else if (pTail > 0)
	{
		instancePool.data[pTail - 1].pNext = handle;
		new_inst->pFormer = pTail;
	}
	new_inst->pNext = 0;
	pTail = handle;
	////
	return handle;
}

void C_Material::DestroyMaterialInstance(size_t _inst_handle)
{
	S_MaterialInst *del_inst = &instancePool.data[_inst_handle - 1];
	del_inst->instNum = nullptr;
	for (size_t id = 0; id < del_inst->parameter_handles.size(); ++id)
	{
		size_t true_id = del_inst->parameter_handles[id];
		if (id != true_id)
		{

			//if (parameters.data[true_id].idleParamHandle > 0)
			//{
			//	FreeIdleParameter(parameterInfos[id].type, parameters.data[true_id].idleParamHandle);
			//}
			parameters.data[true_id].Free();
			parameters.freeData(true_id);
		}
	}
	if (_inst_handle == pHead)
	{
		pHead = del_inst->pNext;
	}
	if (_inst_handle == pTail)
	{
		pTail = del_inst->pFormer;
	}
	if (del_inst->pFormer > 0)
	{
		instancePool.data[del_inst->pFormer - 1].pNext = del_inst->pNext;
	}
	if (del_inst->pNext > 0)
	{
		instancePool.data[del_inst->pNext - 1].pFormer = del_inst->pFormer;
	}
	instancePool.freeData(_inst_handle - 1);
}

void C_Material::SetInstanceParameter(size_t _instance_handle, size_t parameter_data_handle, uint32_t _size, void const * _p_data, bool _self_manage)
{
	S_MaterialInst &inst = instancePool.data[_instance_handle - 1];
	size_t param_data_id = parameter_data_handle;//id of parameter information
	size_t param_pool_handle = 0;
	if (inst.parameter_handles[param_data_id] == param_data_id)
	{
		//instance value is equal to default, allocate new value slot
		inst.parameter_handles[param_data_id] = parameters.assignData();
	}
	size_t param_id = inst.parameter_handles[param_data_id];//id of parameter socket
	parameters.data[param_id].SetParam(_size, _p_data, _self_manage);
}

void C_Material::SetInstanceInstNum(size_t _instance_handle, int *_num)
{
	S_MaterialInst &inst = instancePool.data[_instance_handle - 1];
	inst.instNum = _num;
}

void C_Material::StartInstance()
{
	pCurrent = pHead;
}

void C_Material::NextInstance()
{
	pCurrent = instancePool.data[pCurrent - 1].pNext;
}

bool C_Material::IsInstanceFinished()
{
	return pCurrent == 0;
}