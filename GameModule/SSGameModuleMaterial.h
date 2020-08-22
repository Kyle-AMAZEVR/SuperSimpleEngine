#pragma once
#include <string>


class SSGameModuleMaterial
{
public:
	
	template<class T>
	void SetVSConstantBufferData(std::string name, const T& value);
	template<class T>
	void SetPSConstantBufferData(std::string name, const T& value);

private:
		
};
