#pragma once

class SSFileHelper
{
public:
	static bool FileExists(std::wstring filename);
	static bool DirectoryExists(std::wstring filename);
	static bool MakeDirectory(std::wstring dirname);
	static std::vector<std::string> EnumerateFilesInDirectory(std::string dirname);
};