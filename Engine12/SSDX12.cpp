

#include "SSDX12.h"


void PrintDXError(ID3DBlob* errorMsg)
{
	auto buffSize = errorMsg->GetBufferSize();

	const char* errorMsgPtr = (const char*)errorMsg->GetBufferPointer();

	char* compileErrBuffer = new char[buffSize + 1];

	strcpy_s(compileErrBuffer, buffSize, errorMsgPtr);

	OutputDebugStringA("============= Shader Compile Error =============\n");
	OutputDebugStringA(compileErrBuffer);
	OutputDebugStringA("============= Shader Compile Error =============\n");

	delete[] compileErrBuffer;
}