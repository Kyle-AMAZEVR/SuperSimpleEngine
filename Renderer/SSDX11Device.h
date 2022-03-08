#pragma once

struct SSAdapterInfo
{
	DXGI_ADAPTER_DESC AdapterDesc;
	IDXGIAdapter* AdapterPointer = nullptr;
};

class SSDX11Device
{
public:
	bool CreateDevice();
	ID3D11DeviceContext* GetDeviceContext() const;

	

protected:
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext;
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice;

	std::vector<SSAdapterInfo> mAdapterInfos;
};