#include "Core.h"
#include "SSObjMesh.h"
#include <vector>
#include <string>
#include <cctype>
#include <fstream>

bool SSObjMesh::ImportObjFile(const std::string& FilePath, const std::string& MtlFilePath)
{
	std::ifstream in(FilePath.c_str(), std::ios::in);

	if (in.good())
	{
		std::string Line;
		bool bMtlStart = false;
		unsigned int PreviousMtlVertexIndex = 0;

		while (std::getline(in, Line))
		{
			if (Line.size() <= 1)
			{
				continue;
			}

			if (Line[0] == 'v' && std::isspace(Line[1]))
			{
				//@ Vertex
				SSObjMeshParser::ParseFloat3(Line);
			}
			else if (Line[0] == 'v' && Line[1] == 't')
			{
				//@ Texcoord
			}
			else if (Line[0] == 'v' && Line[1] == 'n')
			{
				//@ Normal
			}
			else if (Line[0] == 'f' && Line[1] == ' ')
			{
				//@Face Info
			}
		}
	}

	return true;
}

XMFLOAT4 SSObjMeshParser::ParseFloat4(std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,
	};

	std::vector<size_t> indexList;

	bool bPrevCharSpace = false;

	for (auto i = 0; i < line.size(); ++i)
	{
		if (std::isspace(line[i]) && bPrevCharSpace == false)
		{
			indexList.push_back(i);
			bPrevCharSpace = true;
		}
		else if (std::isspace(line[i]) == false)
		{
			bPrevCharSpace = false;
		}
	}

	check(indexList.size() == 4);

	XMFLOAT4 result;

	sscanf_s(line.substr(indexList[0], indexList[1]).c_str(), "%f", &result.x);
	sscanf_s(line.substr(indexList[1], indexList[2]).c_str(), "%f", &result.y);
	sscanf_s(line.substr(indexList[2], indexList[3]).c_str(), "%f", &result.z);
	sscanf_s(line.substr(indexList[3], line.size()).c_str(), "%f", &result.w);

	return result;
}


XMFLOAT3 SSObjMeshParser::ParseFloat3 (std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,		
	};

	std::vector<size_t> indexList;

	bool bPrevCharSpace = false;
	
	for (auto i = 0; i < line.size(); ++i)
	{
		if(std::isspace(line[i]) && bPrevCharSpace == false)
		{
			indexList.push_back(i);
			bPrevCharSpace = true;
		}
		else if(std::isspace(line[i]) == false)
		{
			bPrevCharSpace = false;
		}
	}

	check(indexList.size() == 3);

	XMFLOAT3 result;

	sscanf_s(line.substr(indexList[0], indexList[1]).c_str(), "%f", &result.x);
	sscanf_s(line.substr(indexList[1], indexList[2]).c_str(), "%f", &result.y);
	sscanf_s(line.substr(indexList[2], line.size()).c_str(), "%f", &result.z);
	
	return result;
}

XMFLOAT2 SSObjMeshParser::ParseFloat2(std::string& line)
{
	size_t index = 0;

	enum class ParseState
	{
		NONE,
		WHITE_SPACE,
	};

	std::vector<size_t> indexList;

	bool bPrevCharSpace = false;

	for (auto i = 0; i < line.size(); ++i)
	{
		if (std::isspace(line[i]) && bPrevCharSpace == false)
		{
			indexList.push_back(i);
			bPrevCharSpace = true;
		}
		else if (std::isspace(line[i]) == false)
		{
			bPrevCharSpace = false;
		}
	}

	check(indexList.size() == 2);

	XMFLOAT2 result;

	sscanf_s(line.substr(indexList[0], indexList[1]).c_str(), "%f", &result.x);
	sscanf_s(line.substr(indexList[1], line.size()).c_str(), "%f", &result.y);

	return result;
}