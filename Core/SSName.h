

#pragma once

#include "SSCore.h"
#include <map>
#include <unordered_map>
#include <vector>
#include "Singleton.h"

class CORE_API SSName
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

	operator std::string_view() const { return ToStringView();}

	std::string ToString() const;

	std::string_view ToStringView() const;

	friend class SerializeWriter& operator<< (SerializeWriter& Archive, const SSName& name);

	friend class SerializeReader& operator>> (SerializeReader& Archive, SSName& name);


	// bucket index is less than bucket size
	// 
	bool IsValidName() const { return mBucketIndex != 8192; }

protected:
	friend class SSNameBucket;
	size_t mHashValue = 0;
	size_t mBucketIndex = 8192;
};


class CORE_API SSNameBucket : public Singleton<SSNameBucket>
{
public:
	SSNameBucket();
	size_t AddName(size_t hashValue, const std::string& name);
	const std::string& GetName(const SSName* name) const;
	
	void DebugDumpNames();
protected:

	std::vector<std::vector<std::string>> mNameBucket;
	unsigned int mBucketSize = 8192;
};


template<class T>
class TestTemplateClass
{
public:
	static T& Get()
	{
		if (mInstance == nullptr)
		{
			mInstance = new T();
		}
		return *mInstance;
	}
private:
	static T* mInstance;
};

template<class T>
T* TestTemplateClass<T>::mInstance = nullptr;


