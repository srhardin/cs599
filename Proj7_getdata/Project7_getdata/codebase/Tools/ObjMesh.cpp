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
#include "Tools/ObjMesh.h"
#include <fstream>
#include <sstream>

C_ObjMesh::C_ObjMesh()
{
}

C_ObjMesh::~C_ObjMesh()
{
}

int C_ObjMesh::LoadMesh(std::string _path)
{
	std::ifstream inf;
	inf.open(_path, std::ios_base::in);
	if (inf.good())
	{
		std::vector<vec3> vGroup;
		std::vector<vec3> vnGroup;
		std::vector<vec2> vtGroup;
		std::map<VertexData, unsigned int>indicesMap;
		std::string line;
		bool attributes_state[3] = { false };
		while (std::getline(inf, line))
		{
			if (line.length() < 2)
				continue;
			if (line[0] == 'v')
			{
				std::istringstream s(line);
				std::string head;
				if (line[1] == 't')
				{
					//顶点UV
					vec2 newVT(0.0f, 0.0f);
					s >> head >> newVT.s >> newVT.t;
					newVT.t = 1.0f - newVT.t;
					vtGroup.push_back(newVT);
					attributes_state[1] = true;
				}
				else if (line[1] == 'n')
				{
					//顶点法线
					vec3 newVN(0.0f, 0.0f, 0.0f);
					s >> head >> newVN.x >> newVN.y >> newVN.z;
					vnGroup.push_back(newVN);
					attributes_state[2] = true;
				}
				else
				{
					//顶点	
					vec3 newV(0.0f, 0.0f, 0.0f);
					s >> head >> newV.x >> newV.y >> newV.z;
					vGroup.push_back(newV);
					attributes_state[0] = true;
				}
			}
			else if (line[0] == 'f')
			{
				//面
				int face_type = 0;
				line.erase(line.find_last_not_of(" ") + 1);
				for (uint32_t k = line.size() - 1; k > 0; k--)
				{
					if (line[k] == ' ')
						face_type++;
				}
				if (face_type == 3)
				{
					std::string face_string;
					std::istringstream s(line);
					s >> face_string;
					for (uint8_t i = 0; i < 3; i++)
					{
						s >> face_string;
						std::string fs = face_string;
						for (uint32_t k = fs.size() - 1; k > 0; k--)
						{
							if (fs[k] == '/')
							{
								fs[k] = ' ';
							}
						}
						std::istringstream fss(fs);
						uint32_t index;
						VertexData newVertex;
						if (attributes_state[0])
						{
							fss >> index;
							index--;
							newVertex.pos_id = index;
						}
						if (attributes_state[1])
						{
							fss >> index;
							index--;
							newVertex.uv_id = index;
						}
						if (attributes_state[2])
						{
							fss >> index;
							index--;
							newVertex.normal_id = index;
						}
						addVertex(indicesMap,newVertex,vGroup,vnGroup,vtGroup);
					}
				}
				if (face_type == 4)
				{
					std::string face_strings[4];
					VertexData verices[4];
					std::istringstream s(line);
					s >> face_strings[0];
					for (uint8_t i = 0; i < 4; i++)
					{
						s >> face_strings[i];
						std::string fs = face_strings[i];
						for (uint32_t k = fs.size() - 1; k > 0; k--)
						{
							if (fs[k] == '/')
							{
								fs[k] = ' ';
							}
						}
						std::istringstream fss(fs);
						uint32_t index;
						if (attributes_state[0])
						{
							fss >> index;
							index--;
							verices[i].pos_id = index;
						}
						if (attributes_state[1])
						{
							fss >> index;
							index--;
							verices[i].uv_id = index;
						}
						if (attributes_state[2])
						{
							fss >> index;
							index--;
							verices[i].normal_id = index;
						}
					}
					addVertex(indicesMap, verices[0], vGroup, vnGroup, vtGroup);
					addVertex(indicesMap, verices[1], vGroup, vnGroup, vtGroup);
					addVertex(indicesMap, verices[2], vGroup, vnGroup, vtGroup);
					unsigned int zeroIndex = faces.size() - 3;
					faces.push_back(faces[zeroIndex + 2]);
					addVertex(indicesMap, verices[3], vGroup, vnGroup, vtGroup);
					faces.push_back(faces[zeroIndex]);
				}
			}
		}
		inf.close();
		return true;
	}
	return false;
}

int C_ObjMesh::VertexCount(void)
{
	return vertices.size();
}

vec4 * C_ObjMesh::VertexArray(void)
{
	return vertices.data();
}

vec4 * C_ObjMesh::NormalArray(void)
{
	return normals.data();
}

int C_ObjMesh::FaceCount(void)
{
	return faces.size() / 3;
}

unsigned int * C_ObjMesh::FaceArray(void)
{
	return faces.data();
}

vec2 * C_ObjMesh::UVArray(void)
{
	return uv.data();
}

void C_ObjMesh::addVertex(std::map<VertexData, unsigned int>& _search_map, VertexData & new_vertex, std::vector<vec3>& _pgroup, std::vector<vec3>& _ngroup, std::vector<vec2>& _tgroup)
{
	auto result = _search_map.find(new_vertex);
	if (result == _search_map.end())
	{
		int new_index = vertices.size();
		vertices.push_back(vec4(_pgroup[new_vertex.pos_id], 1.0f));
		normals.push_back(vec4(_ngroup[new_vertex.pos_id], 0.0f));
		uv.push_back(_tgroup[new_vertex.pos_id]);
		faces.push_back(new_index);
		_search_map.insert(std::make_pair(new_vertex, new_index));
		return;
	}
	faces.push_back(result->second);
}
