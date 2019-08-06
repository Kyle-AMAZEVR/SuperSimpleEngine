

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include "Singleton.h"

class ENGINE_API SSName
{
public:
	SSName(const std::string& name);
	SSName(const char* name);

	bool operator < (const SSName& rhs) const
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

	bool operator > (const SSName& rhs) const
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

	bool operator == (const SSName& rhs) const
	{
		return mHashValue == mHashValue && mBucketIndex == rhs.mBucketIndex;
	}

	bool operator != (const SSName& rhs) const
	{
		return !(*this == rhs);
	}

	operator std::string() const { return ToString(); }

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
	
	void DebugDumpNames();
protected:

	std::map<size_t, std::vector<std::string>> mNameBucket;

};

