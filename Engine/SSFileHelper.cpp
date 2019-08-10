
#include "Core.h"
#include "SSFileHelper.h"

bool SSFileHelper::FileExists(std::wstring filename)
{
	DWORD result = GetFileAttributesW(filename.c_str());

	if (result != INVALID_FILE_ATTRIBUTES && !(result & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}

	return false;
}

bool SSFileHelper::DirectoryExists(std::wstring filename)
{
	DWORD result = GetFileAttributesW(filename.c_str());

	if (result != INVALID_FILE_ATTRIBUTES && (result & FILE_ATTRIBUTE_DIRECTORY))
	{
		return true;
	}

	return false;
}

bool SSFileHelper::MakeDirectory(std::wstring dirname)
{
	if(SSFileHelper::DirectoryExists(dirname) == false)
	{
		return CreateDirectoryW(dirname.c_str(), nullptr);
	}

	return false;
}
