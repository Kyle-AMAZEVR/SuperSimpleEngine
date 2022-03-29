


#include "SSRendererModulePCH.h"
#include "SSSharedBufferCache.h"
#include "SSSharedRenderData.h"
#include "SSDX11IndexBuffer.h"
#include "SSDX11VertexBuffer.h"

void SSSharedBufferCache::Initialize()
{
	//
	InitializeCube();
	InitializeScreenBlit();
	InitializeSphere();
}

shared_ptr<SSDX11VertexBuffer> SSSharedBufferCache::GetCubeVertexBuffer()
{
	return mCubeVertexBuffer;
}

shared_ptr<SSDX11IndexBuffer> SSSharedBufferCache::GetCubeIndexBuffer()
{
	return mCubeIndexBuffer;
}

shared_ptr<SSDX11VertexBuffer> SSSharedBufferCache::GetSphereVertexBuffer()
{
	return mSphereVertexBuffer;
}

shared_ptr<SSDX11VertexBuffer> SSSharedBufferCache::GetScreenBlitVertexBuffer()
{
	return mScreenBlitVertexBuffer;
}

shared_ptr<SSDX11IndexBuffer> SSSharedBufferCache::GetScreenIndexBuffer()
{
	return mScreenBlitIndexBuffer;
}

void SSSharedBufferCache::InitializeCube()
{
	mCubeIndexBuffer = std::make_shared<SSDX11IndexBuffer>();
	mCubeIndexBuffer->SetIndexBufferData(SSSharedRenderData::Get().GetCubeIndexData());

	mCubeVertexBuffer = std::make_shared<SSDX11VertexBuffer>();
	mCubeVertexBuffer->SetVertexBufferData(SSSharedRenderData::Get().GetCubeVertexData());
}


void SSSharedBufferCache::InitializeSphere()
{
	mSphereVertexBuffer = std::make_shared<SSDX11VertexBuffer>();
	mSphereVertexBuffer->SetVertexBufferData(SSSharedRenderData::Get().GetSphereVertexData());
}


void SSSharedBufferCache::InitializeScreenBlit()
{
	mScreenBlitIndexBuffer = std::make_shared<SSDX11IndexBuffer>();
	mScreenBlitVertexBuffer = std::make_shared<SSDX11VertexBuffer>();

	mScreenBlitVertexBuffer->SetVertexBufferData(SSSharedRenderData::Get().GetScreenBlitVertexData());
	mScreenBlitIndexBuffer->SetIndexBufferData(SSSharedRenderData::Get().GetScreenBlitIndexData());
}