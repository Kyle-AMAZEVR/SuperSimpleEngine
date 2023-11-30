
#include <memory>

class SSRenderCmdBase
{
public:
	virtual void Execute(ID3D11DeviceContext*) = 0;
	
};

class SSRenderCmdSetVS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetVS(std::shared_ptr<class SSVertexShader> inVS)
		:mVS(inVS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;

private:
	std::shared_ptr<SSVertexShader> mVS;
};

class SSRenderCmdSetPS : public SSRenderCmdBase
{
public:
	SSRenderCmdSetPS(std::shared_ptr<class SSDX11PixelShader> inPS)
		:mPS(inPS)
	{
	}

	virtual void Execute(ID3D11DeviceContext* inDeviceContext) override;
private:
	std::shared_ptr<SSDX11PixelShader> mPS;
};