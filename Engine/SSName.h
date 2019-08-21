

#pragma once

#include <map>
#include <unordered_map>
#include <vector>
#include "Singleton.h"
#include "SSISerializable.h"

class ENGINE_API SSName
{
public:
	SSName(){}

	SSName(const std::string& name);

	SSName(const SSName& other);

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

	friend SerializeWriter& operator<< (SerializeWriter& Archive, const SSName& name);
	friend SerializeReader& operator>> (SerializeReader& Archive, SSName& name);

	// bucket index is less than bucket size
	// 
	bool IsValidName() const { return mBucketIndex != 8192; }

protected:
	friend class SSNameBucket;
	size_t mHashValue = 0;
	size_t mBucketIndex = 8192;
};


class ENGINE_API SSNameBucket : public Singleton<SSNameBucket>
{
public:
	SSNameBucket();
	size_t AddName(size_t hashValue, const std::string& name);
	std::string GetName(const SSName* name) const;
	
	void DebugDumpNames();
protected:

	std::vector<std::vector<std::string>> mNameBucket;
	UINT mBucketSize = 8192;

	

};

