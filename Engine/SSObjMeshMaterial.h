

#pragma once


#include <string>

class SSObjMeshMaterial
{
public:

	SSObjMeshMaterial(std::string materialName, std::string diffuse, std::string normal , std::string specular, std::string roughness, std::string mask)
		: mMaterialName(materialName), mDiffuseMap(diffuse), mNormalMap(normal), mSpecularMap(specular), mRoughnessMap(roughness), mMaskMap(mask)
	{		
	}
		
	std::string mMaterialName;
	
	std::string mDiffuseMap;
	
	std::string mNormalMap;
	
	std::string mSpecularMap;
	
	std::string mRoughnessMap;
	
	std::string mMaskMap;
};
