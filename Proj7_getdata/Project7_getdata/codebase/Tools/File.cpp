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
#define CONTENT_PATH "../../Resource/"

namespace file_func
{
	std::string GetPath(std::string subpath)
	{
		std::string return_val = CONTENT_PATH + subpath;
		return return_val;
	}

	std::string LoadTextFile(const char * name)
	{
		std::string return_val = "", line;
		std::ifstream file(name);
		if (!file)
		{
			debug_func::ReportErr("Cannot Load Text File!:\t" + std::string(name));
		}
		do
		{
			line = "";
			getline(file, line);
			return_val += line + "\n";
		} while (file);
		file.close();
		return return_val;
	}

	void * LoadBinaryFile(const char * name)
	{
		return nullptr;
	}

	bool ValidateSuffix(std::string file_name, std::string suffix)
	{
		int pos = file_name.find_last_of('.');
		if (pos < 0)
		{
			return false;
		}
		return file_name.substr(pos) == suffix;
	}

	std::string GetSuffix(std::string file_name)
	{
		int pos = file_name.find_last_of('.');
		if (pos < 0)
		{
			return "";
		}
		return file_name.substr(pos);
	}

void GetCertainFilesInFolder(const std::string& path, std::vector<std::string>& files,std::vector<std::string>& paths,const std::string& fileType,unsigned E_READFILE_FLAG)
	{
		intptr_t hFile = 0;
		_finddata_t fileinfo;

		std::string p;

		if ((hFile = _findfirst(p.assign(path).append("\\*" + fileType).c_str(), &fileinfo)) != -1) {
			do {
				if (strcmp(fileinfo.name, ".") == 0 || strcmp(fileinfo.name, "..") == 0)
					continue;
				if ((E_READFILE_FLAG & E_READFILEPATH) != 0)
				{
					paths.push_back(p.assign(path).append("\\").append(fileinfo.name));
				}
				if((E_READFILE_FLAG & E_READFILENAME) != 0 ){
					files.push_back(fileinfo.name);
				}
			} while (_findnext(hFile, &fileinfo) == 0);

			_findclose(hFile);
		}
	};
	void SearchFilesIncludeSubfolder(const std::string& path, std::vector<std::string>& files,std::vector<std::string>& paths,const std::string& fileType,unsigned E_READFILE_FLAG) {
		//headle
		intptr_t  hFile = 0;
		//file infomation
		struct _finddata_t fileinfo;
		std::string p;
		int nextCode = -1;
		GetCertainFilesInFolder(path, files,paths, fileType, E_READFILE_FLAG);
		if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1) {
			do {
				if ((fileinfo.attrib & _A_SUBDIR)) {
					if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0) {
						//recursive
						SearchFilesIncludeSubfolder(p.assign(path).append("\\").append(fileinfo.name), files,paths,  fileType,E_READFILE_FLAG);
					}
				}
				nextCode = _findnext(hFile, &fileinfo);
			} while (nextCode == 0); //search next, success return 0, fail return -1 
			_findclose(hFile);
		}
	};
	
	
	void* vertical_mirror(void* src, int comp, int w, int h)
	{
		char* csrc = static_cast<char*>(src);
		char *return_val = new char[w * h * comp];
		for (int i = 0; i < h; ++i)
		{
			for (int j = 0; j < w; ++j)
			{
				int psrc = i * w * comp + j * comp;
				int pdst = (h - i - 1) * w * comp + j * comp;
				for (int k = 0; k < comp; ++k)
				{
					return_val[pdst + k] = csrc[psrc + k];
				}
			}
		}
		delete[]csrc;
		return return_val;
	};
}

