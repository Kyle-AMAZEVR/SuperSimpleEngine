#pragma once
#include <DirectXMath.h>


class ENGINE_API SSCameraBase
{
public:
	SSCameraBase()
		: mFOV(DirectX::XM_PI / 6.0f)
		, mAspectRatio(1.0f)
		, mNear (10.f)
		, mFar (10000.0f)
	{
	}

	SSCameraBase(float fov, float aspectRatio, float fNear, float fFar)
		: mFOV(fov), mAspectRatio(aspectRatio), mNear(fNear), mFar(fFar)
	{		
	}

	virtual DirectX::XMMATRIX GetView() const { return mView; }
	virtual DirectX::XMMATRIX GetProj() const { return mProj; }
	virtual DirectX::XMMATRIX GetViewProj() const;

	virtual ~SSCameraBase() {}

	virtual void Update(){}

protected:

	virtual void UpdateViewMatrix(){}
	virtual void UpdateProjMatrix(){}
	
	float mFOV;
	float mAspectRatio;
	float mNear;
	float mFar;

	DirectX::XMMATRIX mView;
	DirectX::XMMATRIX mProj;
};
