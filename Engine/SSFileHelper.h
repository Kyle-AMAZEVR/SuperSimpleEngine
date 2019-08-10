#pragma once

class SSFileHelper
{
public:
	static bool FileExists(std::wstring filename);
	static bool DirectoryExists(std::wstring filename);
	static bool MakeDirectory(std::wstring dirname);
};