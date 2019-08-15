


#include "Core.h"
#include "SSObjMeshMaterial.h"



SerializeWriter& operator << (SerializeWriter& Archive, const SSObjMeshMaterial& Value)
{
	Archive << Value.mMaterialName;
	Archive << Value.mMaskMap;
	Archive << Value.mDiffuseMap;
	Archive << Value.mMetalicMap;
	Archive << Value.mNormalMap;
	Archive << Value.mRoughnessMap;
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshMaterial& Value)
{
	Archive >> Value.mMaterialName;
	Archive >> Value.mMaskMap;
	Archive >> Value.mDiffuseMap;
	Archive >> Value.mMetalicMap;
	Archive >> Value.mNormalMap;
	Archive >> Value.mRoughnessMap;
	return Archive;
}