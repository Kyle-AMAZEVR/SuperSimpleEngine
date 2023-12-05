


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
	std::vector<VT_PositionNormalTexcoordTangent>& VertexData = SSSharedRenderData::Get().GetSphereVertexData();
	mSphereVertexBuffer = SSDX11Renderer::GetDX11Device()->CreateVertexBuffer(sizeof(VT_PositionNormalTexcoordTangent), VertexData.size(), VertexData.data());
}


void SSSharedBufferCache::InitializeScreenBlit()
{
	std::vector<VT_PositionTexcoord>& VertexData = SSSharedRenderData::Get().GetScreenBlitVertexData();
	mScreenBlitVertexBuffer = SSDX11Renderer::GetDX11Device()->CreateVertexBuffer(sizeof(VT_PositionTexcoord), VertexData.size(), VertexData.data());

	std::vector<UINT>& IndexData = SSSharedRenderData::Get().GetScreenBlitIndexData();
	mScreenBlitIndexBuffer = SSDX11Renderer::GetDX11Device()->CreateIndexBuffer(IndexData);
}