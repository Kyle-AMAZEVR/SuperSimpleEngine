
#include "Core.h"
#include <list>


class SSVertexShader;
class SSPixelShader;
class SSSceneObject;
class SSTexture2DBase;
class IRenderTarget;


class ENGINE_API SSDrawCmdBase : public DXRenderResource
{
public :
	virtual void Do() = 0;
};


class ENGINE_API SSChangeRenderTargetCmd : public SSDrawCmdBase
{
public:
	SSChangeRenderTargetCmd(IRenderTarget* renderTarget);
	virtual void Do() override;
protected:
	IRenderTarget* mRenderTarget = nullptr;
};

class ENGINE_API SSDepthStateChangeCmd: public SSDrawCmdBase
{
public:
	SSDepthStateChangeCmd();

protected:

};


class ENGINE_API SSDrawCommand  : public SSDrawCmdBase
{
public:	
	SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSSceneObject> object);

	template<class T>
	void StoreVSConstantBufferData(std::string name, const T& value);

	template<class T>
	void StorePSConstantBufferData(std::string name, const T& value);

	void SetPSTexture(std::string name, SSTexture2DBase* texture);

	void SetVSTexture(std::string name, SSTexture2DBase* texture);	

	void SetPrimitiveType(D3D_PRIMITIVE_TOPOLOGY ePrimitiveType) { mPrimitiveType = ePrimitiveType; }

	D3D_PRIMITIVE_TOPOLOGY GetPrimitiveType() const { return mPrimitiveType; }

	virtual void Do();

protected:
	class SSVertexShader* mpVS = nullptr;
	class SSPixelShader* mpPS = nullptr;	

	//
	std::map<std::string, class SSGenericConstantBuffer*> mVertexShaderConstantBufferMap;
	std::map<std::string, class SSGenericConstantBuffer*> mPixelShaderConstantBufferMap;

	//
	std::map<std::string, class SSTexture2DBase*> mPixelShaderTextureMap;
	std::map<std::string, class SSTexture2DBase*> mVertexShaderTextureMap;

	//
	D3D_PRIMITIVE_TOPOLOGY mPrimitiveType = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	std::shared_ptr<SSSceneObject> mObject;
};

template<class T>
void SSDrawCommand::StoreVSConstantBufferData(std::string name, const T& value)
{
	if (mVertexShaderConstantBufferMap.count(name) > 0)
	{
		mVertexShaderConstantBufferMap[name]->StoreBufferData(value);		
	}
}

template<class T>
void SSDrawCommand::StorePSConstantBufferData(std::string name, const T& value)
{
	if (mPixelShaderConstantBufferMap.count(name) > 0)
	{
		mPixelShaderConstantBufferMap[name]->StoreBufferData(value);		
	}
}
