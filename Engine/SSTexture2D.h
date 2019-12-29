#pragma once

#include "SSTexture2DBase.h"
#include "DirectXTex.h"

class ENGINE_API SSTexture2D : public SSTexture2DBase
{
public:
	SSTexture2D();	
	
	static std::shared_ptr<SSTexture2D> CreateFromDDSFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB=false);
	static std::shared_ptr<SSTexture2D> CreateFromDDSFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bSRGB = false);

	static std::shared_ptr<SSTexture2D> CreateFromHDRFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSTexture2D> CreateFromHDRFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bSRGB = false);

	static std::shared_ptr<SSTexture2D> CreateFromTGAFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB = false);
	static std::shared_ptr<SSTexture2D> CreateFromTGAFile(ID3D11DeviceContext* deviceContext, std::string filename, bool bSRGB = false);

	bool LoadFromDDSFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB = false);
	bool LoadFromTGAFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB = false);
	bool LoadFromHDRFile(ID3D11DeviceContext* deviceContext, std::wstring filename, bool bSRGB = false);

	bool Release();

	UINT GetWidth() const { return mWidth; }
	UINT GetHeight() const { return mHeight; }	
	UINT GetMipLevels() const { return mMipLevels; }
	bool IsSRGB() const { return bSRGB; }

protected:		
	bool LoadInternal(ID3D11DeviceContext* deviceContext, const DirectX::TexMetadata& metaData, const DirectX::ScratchImage& image, bool bsrgb);
	//bool bSRGB = false;
	bool bSRGB = false;

	
};