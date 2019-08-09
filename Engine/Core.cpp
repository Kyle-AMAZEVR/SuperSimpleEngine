

#include "Core.h"


void PrintError(const char* loc, const char* msg)
{	
	OutputDebugStringA(loc);
	OutputDebugStringA("\n");
	OutputDebugStringA(msg);
	OutputDebugStringA("\n");
	MessageBox(nullptr, msg, loc, MB_OK);
}