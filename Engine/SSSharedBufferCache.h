#pragma once

#include <map>
#include "Singleton.h"
#include "SSName.h"

using namespace std;

class SSSharedBufferCache : public Singleton<SSSharedBufferCache>
{
public:
	void Initialize();

	shared_ptr<class SSDX11VertexBuffer> GetCubeVertexBuffer();
	shared_ptr<class SSIndexBuffer> GetCubeIndexBuffer();	
	shared_ptr<class SSDX11VertexBuffer> GetSphereVertexBuffer();
	shared_ptr<SSDX11VertexBuffer> GetScreenBlitVertexBuffer();
	shared_ptr<SSIndexBuffer> GetScreenIndexBuffer();

protected:
	void InitializeCube();
	void InitializeSphere();
	void InitializeScreenBlit();

	std::shared_ptr<SSDX11VertexBuffer> mCubeVertexBuffer;
	std::shared_ptr<SSIndexBuffer> mCubeIndexBuffer;

	std::shared_ptr<SSDX11VertexBuffer> mScreenBlitVertexBuffer;
	std::shared_ptr<SSIndexBuffer> mScreenBlitIndexBuffer;

	
	
};
