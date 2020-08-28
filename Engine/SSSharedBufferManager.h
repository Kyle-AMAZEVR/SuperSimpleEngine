#pragma once

#include <map>
#include "Singleton.h"
#include "SSName.h"

class SSSharedBufferManager : Singleton<SSSharedBufferManager>
{
public:
	void Initialize();

protected:
	std::map<SSName, class SSDX11VertexBuffer*> mVertexBufferMap;
	std::map<SSName, class SSIndexBuffer*> mIndexBufferMap;	
};
