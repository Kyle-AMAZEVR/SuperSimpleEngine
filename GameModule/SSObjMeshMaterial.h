

#pragma once

#include "SSGameModule.h"
#include <string>
#include "SSISerializable.h"

class GAMEMODULE_API SSObjMeshMaterial
{
public:
	SSObjMeshMaterial(){}
	SSObjMeshMaterial(std::string materialName, std::string diffuse, std::string normal , std::string metalic, std::string roughness, std::string mask)
		: mMaterialName(materialName), mDiffuseMap(diffuse), mNormalMap(normal), mMetalicMap(metalic), mRoughnessMap(roughness), mMaskMap(mask)
	{		
	}
		
	std::string mMaterialName;
	
	std::string mDiffuseMap;
	
	std::string mNormalMap;
	
	std::string mMetalicMap;
	
	std::string mRoughnessMap;
	
	std::string mMaskMap;

	friend SerializeWriter& operator << (SerializeWriter& Archive, const class SSObjMeshMaterial& Value);
	friend SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshMaterial& Value);
};


SerializeWriter& operator << (SerializeWriter& Archive, const class SSObjMeshMaterial& Value);
SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshMaterial& Value);
