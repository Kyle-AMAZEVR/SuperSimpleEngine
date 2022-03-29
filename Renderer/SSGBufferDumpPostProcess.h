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
 
	virtual void Draw(SSDX11Device* renderDevice, SSDX11Texture2D* input0, SSDX11Texture2D* input1, SSDX11Texture2D* input2) override;

	void ChangeNextDumpMode();

protected:
	std::shared_ptr<SSVertexShader> mVertexShader;
	std::shared_ptr<SSPixelShader> mPixelShader;

	EDumpMode mDumpMode = EDumpMode::EPositionDump;
	//EDumpMode mDumpMode = EDumpMode::EMetalicDump;
	void UpdateDumpSettings();


	
};
