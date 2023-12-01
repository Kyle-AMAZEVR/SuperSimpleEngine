
#include <memory>

class SSRenderCmdBase
{
public:
	virtual void Execute(ID3D11DeviceContext*) = 0;
	
};

class SSRenderCmdSetVS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVS(class SSDX11VertexShader* inVS)
		:mVS(inVS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	SSDX11VertexShader* mVS;
};

class SSRenderCmdSetPS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPS(class SSDX11PixelShader* inPS)
		:mPS(inPS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	SSDX11PixelShader* mPS;
};

class SSRenderCmdSetVSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSTexture(class SSDX11VertexShader* inVS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSDX11VertexShader* mVS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSTexture(class SSDX11PixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSDX11PixelShader* mPS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetVSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSCBuffer(SSDX11VertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11VertexShader* mVS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSCBuffer(SSDX11PixelShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11PixelShader* mPS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdCopyCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdCopyCBuffer(SSDX11Buffer* ptrBuffer);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSDX11Buffer* mBuffer;
};

class SSRenderCmdSetRenderTarget : public SSRenderCmdBase
{
public:
	SSRenderCmdSetRenderTarget(class SSDX11RenderTarget* inRT);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	SSDX11RenderTarget* mRenderTarget;
};

class SSRenderCmdClearRenderTarget : public SSRenderCmdBase
{
public:

};