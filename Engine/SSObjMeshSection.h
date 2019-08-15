#pragma once

#include <string>
#include "SSISerializable.h"


class SSObjMeshSection
{	
public:
	SSObjMeshSection(){}
	SSObjMeshSection(std::string sectionName, UINT start)
		: mSectionName(sectionName), mStartIndex(start)
	{		
	}
	SSObjMeshSection(std::string sectionName, UINT start, UINT end)
		: mSectionName(sectionName), mStartIndex(start), mEndIndex(end)
	{	
	}

	void SetEndIndex(UINT endIndex) { mEndIndex = endIndex; }
protected:
	friend SerializeWriter& operator << (SerializeWriter& Archive, const class SSObjMeshSection& Value);
	friend SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshSection& Value);
	
	UINT mStartIndex = 0;

	UINT mEndIndex = 0;

	std::string mSectionName;
};

SerializeWriter& operator << (SerializeWriter& Archive, const SSObjMeshSection& Value);
SerializeReader& operator >> (SerializeReader& Archive, SSObjMeshSection& Value);