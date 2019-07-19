#pragma once


class ENGINE_API DXCube : public DXRenderResource
{
public:
	DXCube();

protected:
	void InternalCreate();

	static std::unique_ptr<class DXVertexBuffer> mCubeVB;
	static std::unique_ptr<class DXIndexBuffer> mCubeIB;
};