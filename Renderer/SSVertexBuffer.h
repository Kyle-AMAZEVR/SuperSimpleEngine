#pragma once


#include "SSBufferBase.h"

class DX11RENDERER_API SSVertexBuffer : public SSBufferBase
{
public:
	SSVertexBuffer();
	virtual ~SSVertexBuffer();

	virtual unsigned int GetStride() const { return mStride; }
	virtual unsigned int GetVertexCount() const { return mVertexCount; }
	virtual unsigned int GetTotalSize() const { return mTotalSize; }

protected:
	unsigned int mStride = 0;
	unsigned int mVertexCount = 0;
	unsigned int mTotalSize = 0;
};
