#pragma once



class ENGINE_API Translator
{
public:
	static DXGI_FORMAT GetVertexShaderInputType(D3D11_SIGNATURE_PARAMETER_DESC desc);
	static UINT GetDXGIFormatByteSize(DXGI_FORMAT format);
};