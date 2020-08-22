
#include "SSCommon.h"
#include "SSName.h"
#include "SSFreeCamera.h"


SSName::SSName(const SSName& other)
{
	mHashValue = other.mHashValue;
	mBucketIndex = other.mBucketIndex;
}

SSName::SSName(const std::string& name)
{
	mHashValue = std::hash<std::string>{} {name};
	mBucketIndex = SSNameBucket::Get().AddName(mHashValue, name);
}

SSName::SSName(const char* name)
{
	std::string strName = name;
	mHashValue = std::hash<std::string>{} {strName};
	mBucketIndex = SSNameBucket::Get().AddName(mHashValue, strName);
}

std::string SSName::ToString() const 
{
	return SSNameBucket::Get().GetName(this);
}

//

SSNameBucket::SSNameBucket()
{	
	mNameBucket.resize(mBucketSize);
}


size_t SSNameBucket::AddName(size_t hashValue, const std::string& name)
{
	UINT bucketIndex = hashValue % mBucketSize;

	if (mNameBucket[bucketIndex].size() > 0)
	{
		auto& bucket = mNameBucket[bucketIndex];
		
		for (size_t i = 0; i < bucket.size(); ++i)
		{
			if (bucket[i] == name)
			{
				return i;
			}
		}

		bucket.push_back(name);
		return bucket.size() - 1;
	}
	else
	{
		std::vector<std::string> temp;
		mNameBucket[bucketIndex] = temp;
		mNameBucket[bucketIndex].push_back(name);
		return 0;
	}	
}

std::string SSNameBucket::GetName(const SSName* name) const
{
	auto& Ptr = TestTemplateClass<double>::Get();

	UINT bucketIndex = name->mHashValue % mBucketSize;

	if (mNameBucket[bucketIndex].size() > 0)
	{
		const auto& v = mNameBucket[bucketIndex];
		return v[name->mBucketIndex];
	}
	else
	{
		return "";
	}
}

