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
#include "Tools/LuaHelper.h"

namespace debug_func
{
	void CheckLuaErr(lua_State * L, int state)
	{
		if (state != 0)
		{
			std::cout << std::string(lua_tostring(L, state)) << std::endl;
		}
	}
}