

#pragma once

class ENGINE_API SSViewport 
{
public:
    int GetWidth() const { return mWidth; }
    void Clear();
    void Resize(int newWidth, int newHeight);

	void MakeCurrent();

protected:
    int mWidth = 0;
    int mHeight = 0;

    ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
    D3D11_VIEWPORT mScreenViewport;
};