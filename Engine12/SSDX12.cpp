

#include "SSDX12.h"


void PrintError(const char* loc, const char* msg)
{	
	OutputDebugStringA(loc);
	OutputDebugStringA("\n");
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
	MessageBox(nullptr, msg, loc, MB_OK);
}

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