#include "SSRendererModulePCH.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"



std::shared_ptr<class SSTexture2D> SSTextureManager::LoadTexture2D(ID3D11DeviceContext* deviceContext, SSName texturepath, bool bSRGB)
{
	if (m2DTextureMap.count(texturepath) > 0)
	{
		return m2DTextureMap[texturepath];
	}
	else
	{
		std::string path = texturepath.ToString();
		std::shared_ptr<SSTexture2D> texture;
		
		if (path.find(".dds") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromDDSFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
		else if (path.find(".tga") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromTGAFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
		else if (path.find(".hdr") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromHDRFile( path, bSRGB);

			if (texture == nullptr)
			{
				check(false);
				return nullptr;
			}

			m2DTextureMap[path] = texture;

			return texture;
		}
	}

	return nullptr;
}