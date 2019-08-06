
#include "Core.h"
#include "SSName.h"

SSName::SSName(const std::string& name)
{
	mHashValue = std::hash<std::string>{} {name};
	mBucketIndex = SSNameBucket::Get().AddName(mHashValue, name);
}

std::string SSName::ToString() const 
{
	return SSNameBucket::Get().GetName(this);
}

//

size_t SSNameBucket::AddName(size_t hashValue, const std::string& name)
{
	if (mNameBucket.count(hashValue) > 0)
	{
		auto& bucket = mNameBucket[hashValue];
		
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
		mNameBucket[hashValue] = temp;
		mNameBucket[hashValue].push_back(name);
		return 0;
	}	
}

std::string SSNameBucket::GetName(const SSName* name) const
{
	if (mNameBucket.count(name->mHashValue) > 0)
	{
		const auto& v = mNameBucket.at(name->mHashValue);
		return v[name->mBucketIndex];
	}
	else
	{
		return "";
	}
}