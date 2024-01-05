
#include "SSRendererModulePCH.h"
#include "SSDX11ConstantBuffer.h"
#include "SSObjectBase.h"
#include "SSName.h"
#include <functional>
#include <map>

class SSDX11VertexShader;
class SSDX11PixelShader;
class SSGameObject;
class SSDX11Texture2D;
class IRenderTarget;


class SSDrawCmdBase
{
public :	
	virtual void Do(SSDX11Device* deviceContext) = 0;
};


class SSChangeRenderTargetCmd : public SSDrawCmdBase
{
public:
	SSChangeRenderTargetCmd(IRenderTarget* renderTarget);
	
	virtual void Do(SSDX11Device* deviceContext) override;
protected:
	IRenderTarget* mRenderTarget = nullptr;
};

class SSDepthStateChangeCmd: public SSDrawCmdBase
{
public:
	SSDepthStateChangeCmd();
protected:
	
};


class SSDrawCommand  : public SSDrawCmdBase
{
public:
	//SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSObjectBase> object);
	SSDrawCommand(std::shared_ptr<SSDX11VertexShader> vs, std::shared_ptr<SSDX11PixelShader> ps, std::shared_ptr<SSObjectBase> object);

	template<class T>
	void StoreVSConstantBufferData(SSName name, const T& value);

	template<class T>
	void StorePSConstantBufferData(SSName name, const T& value);

	void SetPSTexture(std::string name, SSDX11Texture2D* texture);

	void SetVSTexture(std::string name, SSDX11Texture2D* texture);

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }

	virtual void Do(SSDX11Device* deviceContext) override;

protected:
	std::shared_ptr<class SSDX11VertexShader> mpVS = nullptr;
	std::shared_ptr<class SSDX11PixelShader> mpPS = nullptr;
	class SSMaterial* mMaterial = nullptr;	

	//
	std::map<SSName, class SSDX11ConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<SSName, class SSDX11ConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSDX11Texture2D*> mPixelShaderTextureMap;
	std::map<std::string, class SSDX11Texture2D*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
	
	std::shared_ptr<SSObjectBase> mObject;
};

template<class T>
void SSDrawCommand::StoreVSConstantBufferData(SSName name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->SetBufferData((void*) &value, sizeof(T));
	}
}

template<class T>
void SSDrawCommand::StorePSConstantBufferData(SSName name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->SetBufferData((void*)&value, sizeof(T));
	}
}
