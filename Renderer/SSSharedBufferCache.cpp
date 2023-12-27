


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

void SSSharedBufferCache::Shutdown()
{
	mCubeIndexBuffer.reset();
	mScreenBlitIndexBuffer.reset();
	mScreenBlitVertexBuffer.reset();
	mSphereVertexBuffer.reset();
	mCubeVertexBuffer.reset();	
}

void SSSharedBufferCache::InitializeCube()
{	
	std::vector<unsigned int>& IndexData = SSSharedRenderData::Get().GetCubeIndexData();
	mCubeIndexBuffer = SSDX11Renderer::GetDX11Device()->CreateIndexBuffer(IndexData);
		
	std::vector<VT_PositionNormalTexcoordTangent>& VertexData = SSSharedRenderData::Get().GetCubeVertexData();
	mCubeVertexBuffer = SSDX11Renderer::GetDX11Device()->CreateVertexBuffer(
		sizeof(VT_PositionNormalTexcoordTangent), 
		static_cast<unsigned int>(VertexData.size()), 
		VertexData.data());
}


void SSSharedBufferCache::InitializeSphere()
{
	std::vector<VT_PositionNormalTexcoordTangent>& VertexData = SSSharedRenderData::Get().GetSphereVertexData();
	mSphereVertexBuffer = SSDX11Renderer::GetDX11Device()->CreateVertexBuffer(
		sizeof(VT_PositionNormalTexcoordTangent), 
		static_cast<unsigned int>(VertexData.size()),
		VertexData.data());
}


void SSSharedBufferCache::InitializeScreenBlit()
{
	std::vector<VT_PositionTexcoord>& VertexData = SSSharedRenderData::Get().GetScreenBlitVertexData();
	mScreenBlitVertexBuffer = SSDX11Renderer::GetDX11Device()->CreateVertexBuffer(
		sizeof(VT_PositionTexcoord), 
		static_cast<unsigned int>(VertexData.size()),
		VertexData.data());

	std::vector<UINT>& IndexData = SSSharedRenderData::Get().GetScreenBlitIndexData();
	mScreenBlitIndexBuffer = SSDX11Renderer::GetDX11Device()->CreateIndexBuffer(IndexData);
}