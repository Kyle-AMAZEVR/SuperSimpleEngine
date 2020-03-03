#pragma once



class COMMON_API SSDXTranslator
{
public:
	static DXGI_FORMAT GetVertexShaderInputType(D3D11_SIGNATURE_PARAMETER_DESC desc);
	static DXGI_FORMAT GetVertexShaderInputType(D3D12_SIGNATURE_PARAMETER_DESC desc);
	static UINT GetDXGIFormatByteSize(DXGI_FORMAT format);

private:
	static DXGI_FORMAT GetVertexShaderInputType(D3D_REGISTER_COMPONENT_TYPE type, const UINT componentCount);
};