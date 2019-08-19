#pragma once
#include "SSPostProcess.h"
#include "FreqUsedConstantBufferTypes.h"
#include "SSShader.h"


enum class EDumpMode : unsigned char
{
	EPositionDump,
	ENormalDump,
	EMetalicDump,
	EDiffuseDump,
	ERoughnessDump,
};

class SSGBufferDumpPostProcess : public SSPostProcess
{
public:
	SSGBufferDumpPostProcess(UINT width, UINT height);
 
	virtual void Draw(SSTexture2DBase* input0, SSTexture2DBase* input1, SSTexture2DBase* input2) override;

	void ChangeNextDumpMode();

protected:
	std::shared_ptr<SSVertexShader> mVertexShader;
	std::shared_ptr<SSPixelShader> mPixelShader;

	EDumpMode mDumpMode = EDumpMode::EPositionDump;
	//EDumpMode mDumpMode = EDumpMode::EMetalicDump;
	void UpdateDumpSettings();


	
};
