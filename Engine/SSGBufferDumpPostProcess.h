#pragma once
#include "SSPostProcess.h"

#include "SSShader.h"


class SSGBufferDumpPostProcess : public SSPostProcess
{
public:

protected:

	std::shared_ptr<SSVertexShader> mVertexShader;
};
