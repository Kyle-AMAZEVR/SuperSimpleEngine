
#include "Core.h"
#include <list>

class ENGINE_API SSDrawCommand 
{
public:



	virtual void Do();

protected:
	std::shared_ptr<class SSVertexShader> mVS;
	std::shared_ptr<class SSPixelShader> mPS;
	std::shared_ptr<class SSVertexBuffer> mVB;
	std::shared_ptr<class SSIndexBuffer> mIB;	

	std::vector<std::shared_ptr<class SSBufferBase>> mVertexShaderConstantBufferList;
	std::vector<std::shared_ptr<class SSBufferBase>> mPixelShaderConstantBufferList;
};