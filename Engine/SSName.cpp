
#include "Core.h"
#include "SSName.h"

SSName::SSName(std::string name)
{
	mHashValue = std::hash<std::string>{} {name};
}
