
#include "Core.h"
#include <list>


class SSVertexShader;
class SSPixelShader;
class SSSceneObject;

class ENGINE_API SSDrawCommand  : public DXRenderResource
{
public:	
	SSDrawCommand(SSVertexShader* vs, SSPixelShader* ps, std::shared_ptr<SSSceneObject> object);
	
	//template<class T>
	//void SetVSConstantBufferData(std::string name, const T& value);


	virtual void Do();

protected:
	class SSVertexShader* mpVS = nullptr;
	class SSPixelShader* mpPS = nullptr;

	//
	std::map<std::string, class SSBufferBase*> mVertexShaderConstantBufferMap;
	std::map<std::string, class SSBufferBase*> mPixelShaderConstantBufferMap;

	//


	std::shared_ptr<SSSceneObject> mObject;
};
