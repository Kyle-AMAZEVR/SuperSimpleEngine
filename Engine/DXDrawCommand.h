
#include "Core.h"
#include <list>

class ENGINE_API SSDrawCommand 
{
public:



	virtual void Do();

protected:
	std::shared_ptr<class DXVertexShader> mVS;
	std::shared_ptr<class DXPixelShader> mPS;
	std::shared_ptr<class DXVertexBuffer> mVB;
	std::shared_ptr<class DXIndexBuffer> mIB;	

	std::vector<std::shared_ptr<class DXBufferBase>> mVertexShaderConstantBufferList;
	std::vector<std::shared_ptr<class DXBufferBase>> mPixelShaderConstantBufferList;
};