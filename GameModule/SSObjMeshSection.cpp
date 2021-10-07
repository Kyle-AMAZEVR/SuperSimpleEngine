
#include "SSObjMeshSection.h"


SerializeWriter& operator << (SerializeWriter& Archive, const SSObjMeshSection& Value)
{
	Archive << Value.mStartIndex;
	Archive << Value.mEndIndex;
	Archive << Value.mSectionName;
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshSection& Value)
{
	Archive >> Value.mStartIndex;
	Archive >> Value.mEndIndex;
	Archive >> Value.mSectionName;
	return Archive;
}