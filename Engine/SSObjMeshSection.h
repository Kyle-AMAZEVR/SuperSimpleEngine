#pragma once

#include <string>


class SSObjMeshSection
{	
public:
	SSObjMeshSection(std::string sectionName, UINT start, UINT end)
		: mSectionName(sectionName), mStartIndex(start), mEndIndex(end)
	{	
	}


protected:
	UINT mStartIndex = 0;

	UINT mEndIndex = 0;

	std::string mSectionName;
};