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
//#include "Tools/Debug.h"
//#include "Runtime/LuaScripts.h"
//#define DEBUG_OUTPUT

namespace debug_func
{

	void ReportErr(std::string err_string)
	{
#ifdef DEBUG_OUTPUT
		fprintf(stderr, "Error: %s\n", err_string.c_str());
#endif
	}

	void ReportWarning(std::string warning_string)
	{
#ifdef DEBUG_OUTPUT
		fprintf(stderr, "Warning: %s\n", warning_string.c_str());
#endif
	}

	void CheckErr(bool check, std::string err_string)
	{
#ifdef DEBUG_OUTPUT
		if (!check)
		{
			fprintf(stderr, "Error: %s\n", err_string.c_str());
		}
#endif
	}

	void CheckGlewErr(GLenum result)
	{
#ifdef DEBUG_OUTPUT
		if (GLEW_OK != result)
		{
			fprintf(stderr, "Error: %s\n", glewGetErrorString(result));
		}
#endif
	}

	void Report(std::string report_string)
	{
#ifdef DEBUG_OUTPUT
		if (true)
		{
			fprintf(stdout, "%s\n", report_string.c_str());
		}
#endif
	}

}