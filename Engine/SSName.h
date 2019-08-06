

#pragma once

#include <unordered_map>

class ENGINE_API SSName
{
public:
	SSName(std::string name);
	SSName(std::wstring name);

	

protected:

	size_t mHashValue = 0;
};


