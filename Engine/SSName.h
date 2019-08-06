

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include "Singleton.h"

class ENGINE_API SSName
{
public:
	SSName(const std::string& name);	

	bool operator < (const SSName& rhs)
	{
		if (mHashValue < rhs.mHashValue)
		{
			return true;
		}

		if (mBucketIndex < rhs.mBucketIndex)
		{
			return true;
		}

		return false;
	}

	bool operator > (const SSName& rhs)
	{	
		if (mHashValue > rhs.mHashValue)
		{
			return true;
		}

		if (mBucketIndex > rhs.mBucketIndex)
		{
			return true;
		}		

		return false;
	}

	bool operator == (const SSName& rhs)
	{
		return mHashValue == mHashValue && mBucketIndex == rhs.mBucketIndex;
	}

	bool operator != (const SSName& rhs)
	{
		return !(*this == rhs);
	}

	std::string ToString() const;

protected:
	friend class SSNameBucket;
	size_t mHashValue = 0;
	size_t mBucketIndex = 0;
};


class ENGINE_API SSNameBucket : public Singleton<SSNameBucket>
{
public:
	size_t AddName(size_t hashValue, const std::string& name);
	std::string GetName(const SSName* name) const;

protected:

	std::map<size_t, std::vector<std::string>> mNameBucket;

};

