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
#include "System/LuaScriptSys.h"
#include "Component/LuaComponent.h"
#include "../LuaAPI/LuaVector.h"
#include "../LuaAPI/DarkFrostLua.h"
#include "../LuaAPI/ChunYanLua.h"
#include "../LuaAPI/AkiLua.h"
#include "../LuaAPI/Cardinal.h"
#include "../LuaAPI/LuaEventSystem.h"
#include "Entity.h"
#include "Config.h"
#include <direct.h>
extern "C"
{
#include"lua.h"
#include "lauxlib.h"  
#include "lualib.h" 
}

namespace
{
	luaL_Reg lua_registration_lib_list[] =
	{
		{"System",luaAPI::lua_reg_System_lib},
		{"InputManager", luaAPI::lua_reg_input_mgr_lib},
		{"Vector", luaAPI::lua_reg_vector_lib},
		{"CameraComponent", luaAPI::lua_reg_camera_lib},
		{"MaterialComponent", luaAPI::lua_reg_material_lib},
		{"ParticleComponent", luaAPI::lua_reg_particle_lib},
		{"Body", luaAPI::lua_reg_body_lib},
		{"Animation", luaAPI::lua_reg_animation_lib},
		{"Player", luaAPI::lua_reg_player_lib},
		{"Transform", luaAPI::lua_reg_transform_lib},
		{"Utility", luaAPI::lua_reg_utility_lib},
		{"CEvent", luaAPI::lua_reg_event_lib},
		{"Script", luaAPI::lua_reg_script_lib},
		{"Renderer", luaAPI::lua_reg_renderer_lib},
		{"Audio", luaAPI::lua_reg_Audio_lib},
		{"Collider", luaAPI::lua_reg_Collider_lib},
		{"GameManager", luaAPI::lua_reg_gameManager_lib},
		{"Player_type2", luaAPI::lua_reg_player_lib},
		{"QuaternionExt", luaAPI::lua_reg_qurternion_ext_lib},
		{"Tag", luaAPI::lua_reg_tag_lib},
		{"Text", luaAPI::lua_reg_text_lib},
	};
}

C_LuaScriptSys::C_LuaScriptSys() : eventSystem(new C_LuaEventSystem())
{
	L = luaL_newstate();
	luaL_openlibs(L);
	lua_getglobal(L, "package");
	lua_getfield(L, -1, "path");
	std::string cur_path = lua_tostring(L, -1);
	//cur_path.append();
	std::string root(_getcwd(NULL, 0));
	cur_path.append(";" + root + "/" + file_func::GetPath("LuaScript/?.lua") + ";" + root + "/Bin/InternalLuaLibraries/?.lua");
	lua_pop(L, 1);
	lua_pushstring(L, cur_path.c_str());
	lua_setfield(L, -2, "path");
	//register libraries no library for now
	for (int i = 0; i < sizeof(lua_registration_lib_list) / sizeof(luaL_Reg); ++i)
	{
		luaL_requiref(L, lua_registration_lib_list[i].name, lua_registration_lib_list[i].func, 0);
		lua_pop(L, 1);
	}
	lua_settop(L, 0);
	eventSystem->InitEventSystem(L);
}

C_LuaScriptSys::~C_LuaScriptSys()
{
	delete eventSystem;
}

void C_LuaScriptSys::Register(C_World & worldname, std::vector<int>& entityList)
{
	if (entityList.size() == 0) return;

	for (int &i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_SCRIPT)).any())
		{
			int id = worldname.mEntities[i].mComponents[E_CP_SCRIPT];
			mScriptList.insert(id);
			std::string script_store_name = std::string("SN") + to_string(id);
			//create component table
			lua_newtable(L);
			lua_setfield(L, LUA_REGISTRYINDEX, script_store_name.c_str());
			lua_getfield(L, LUA_REGISTRYINDEX, script_store_name.c_str());
			C_LuaComponent &new_lua = worldname.mLuaComponents[id];

			for (int id = 0; id < new_lua.names.size(); ++id)
			{
				std::string path = file_func::GetPath("LuaScript/" + new_lua.names[id]);
				if (luaL_loadfile(L, path.c_str()))
				{
					debug_func::ReportErr("Failed to load script " + path);
				}
				//encapsulize file functions into class
				lua_newtable(L);
				lua_pushinteger(L, i);
				lua_setfield(L, -2, "ENTITY_ID");
				lua_newtable(L);
				lua_getglobal(L, "_G");
				lua_setfield(L, -2, "__index");
				lua_setmetatable(L, -2);
				lua_setfield(L, -3, new_lua.names[id].c_str());
				lua_getfield(L, -2, new_lua.names[id].c_str());
				lua_setupvalue(L, -2, 1);

				//int i = 0;
				debug_func::CheckLuaErr(L, lua_pcall(L, 0, 0, 0));
			}
			
			lua_settop(L, 0);
		}
	}
}

void C_LuaScriptSys::Unregister(C_World & worldname, std::vector<int>& entityList)
{
	for (auto i : entityList)
	{
		if ((worldname.mEntities[i].entityMask & std::bitset<64>(E_CPM_SCRIPT)).any())
		{
			eventSystem->EraseEntityEvent(L, i);
			int id = worldname.mEntities[i].mComponents[E_CP_SCRIPT];
			mScriptList.erase(worldname.mEntities[i].mComponents[E_CP_SCRIPT]);
			std::string script_store_name = std::string("SN") + to_string(id);
			//std::string uniq_name = std::to_string((uintptr_t)unique_address);
			lua_getfield(L, LUA_REGISTRYINDEX, script_store_name.c_str());
			if (!lua_isnil(L, -1))
			{
				lua_getfield(L, -1, "destructor");
				if (!lua_isnil(L, -1))
				{
					debug_func::CheckLuaErr(L, lua_pcall(L, 0, 0, 0));
				}
				lua_pushnil(L);
				lua_setfield(L, LUA_REGISTRYINDEX, script_store_name.c_str());
			}
			lua_settop(L, 0);
		}
	}
}

void C_LuaScriptSys::Update(C_World & worldname, float deltaTime)
{
	auto itr = mScriptList.begin();
	auto itr_end = mScriptList.end();
	while (itr != itr_end)
	{
		std::string script_store_name = std::string("SN") + to_string(*itr);
		lua_getfield(L, LUA_REGISTRYINDEX, script_store_name.c_str());
		if (!lua_isnil(L, -1))
		{
			
			//int table_length = lua_rawlen(L, -1);
			////lua_pop(L, 1);
			//for (int script_itr = 0; script_itr < table_length; ++script_itr)
			//{
			//	lua_rawgeti(L, -1, script_itr + 1);
			//	lua_getfield(L, -1, "update");
			//	if (!lua_isnil(L, -1))
			//	{
			//		lua_pushnumber(L, deltaTime);
			//		debug_func::CheckLuaErr(L, lua_pcall(L, 1, 0, 0));
			//	}
			//	else
			//	{
			//		lua_pop(L, 1);
			//	}
			//	lua_pop(L, 1);
			//}

			lua_pushnil(L);			
			while (lua_next(L, -2) != 0)
			{
				lua_getfield(L, -1, "update");
				if (!lua_isnil(L, -1))
				{
					lua_pushnumber(L, deltaTime);
					debug_func::CheckLuaErr(L, lua_pcall(L, 1, 0, 0));
				}
				else
				{
					lua_pop(L, 1);
				}
				lua_pop(L, 1);
			}
		}
		lua_settop(L, 0);
		++itr;
	}
	eventSystem->Tick(L, deltaTime);
}
