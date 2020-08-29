#pragma once

#include <map>
#include "Singleton.h"
#include "SSName.h"

using namespace std;

class SSSharedBufferCache : Singleton<SSSharedBufferCache>
{
public:
	void Initialize();

	shared_ptr<class SSDX11VertexBuffer> GetCachedCubeVertexBuffer();
	shared_ptr<class SSIndexBuffer> GetCubeIndexBuffer();

	shared_ptr<class SSDX11VertexBuffer> GetCachedSphereVertexBuffer();

protected:
	void InitializeCube();
	void InitializeSphere();
	
	std::map<SSName, class SSDX11VertexBuffer*> mVertexBufferMap;
	std::map<SSName, class SSIndexBuffer*> mIndexBufferMap;	
};
