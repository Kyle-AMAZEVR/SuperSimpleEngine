
#include <memory>

class SSRenderCmdBase
{
public:
	virtual void Execute(ID3D11DeviceContext*) = 0;
	
};

class SSRenderCmdSetVS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVS(class SSVertexShader* inVS)
		:mVS(inVS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	SSVertexShader* mVS;
};

class SSRenderCmdSetPS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPS(class SSPixelShader* inPS)
		:mPS(inPS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	SSPixelShader* mPS;
};

class SSRenderCmdSetVSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSTexture(class SSVertexShader* inVS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSVertexShader* mVS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSTexture : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSTexture(class SSPixelShader* inPS, class SSDX11Texture2D* inTex, unsigned int slotIndex);

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

protected:
	SSPixelShader* mPS;
	SSDX11Texture2D* mTex;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetVSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVSCBuffer(SSVertexShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSVertexShader* mVS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};

class SSRenderCmdSetPSCBuffer : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPSCBuffer(SSPixelShader* inVS, class SSDX11Buffer* inBuffer, unsigned int slot);
	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
protected:
	SSPixelShader* mPS;
	SSDX11Buffer* mBuffer;
	unsigned int mSlotIndex;
};