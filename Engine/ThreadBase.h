#pragma once

#include "Windows.h"

class ENGINE_API ThreadBase
{
public:
	void Start();
	void Join();
	static DWORD Run(LPVOID param);	
protected:
	DWORD RunInternal();
	HANDLE mThreadHandle;
};