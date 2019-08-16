#include "Core.h"
#include "SSTextureManager.h"
#include "SSTexture2D.h"


bool SSTextureManager::LoadTexture2D(SSName texturepath)
{
	if (m2DTextureMap.count(texturepath) > 0)
	{
		return true;
	}
	else
	{
		std::string path = texturepath.ToString();
		std::shared_ptr<SSTexture2D> texture;
		
		if (path.find(".dds") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromDDSFile(path);

			if (texture == nullptr)
			{
				return false;
			}

			m2DTextureMap[path] = texture;
		}
		else if (path.find(".tga") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromTGAFile(path);

			if (texture == nullptr)
			{
				return false;
			}

			m2DTextureMap[path] = texture;
		}
		else if (path.find(".hdr") != std::string::npos)
		{
			texture = SSTexture2D::CreateFromHDRFile(path);

			if (texture == nullptr)
			{
				return false;
			}

			m2DTextureMap[path] = texture;
		}
	}
}