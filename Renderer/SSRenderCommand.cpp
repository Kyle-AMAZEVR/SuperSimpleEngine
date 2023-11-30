
#include "SSRenderCommand.h"
#include "SSShader.h"

void SSRenderCmdSetVS::Execute(ID3D11DeviceContext * inDeviceContext)
{
	inDeviceContext->VSSetShader(mVS->GetShader(), nullptr, 0);
}

