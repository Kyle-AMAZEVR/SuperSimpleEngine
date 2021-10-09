#pragma once

#include <vector>

class CORE_API SSFileHelper
{
public:
	static bool FileExists(std::wstring filename);
	static bool DirectoryExists(std::wstring filename);
	static bool MakeDirectory(std::wstring dirname);
	static std::vector<std::string> EnumerateFilesInDirectory(std::string dirname);
};