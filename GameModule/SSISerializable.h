#pragma once

#include <fstream>
#include <vector>
#include <map>


using namespace DirectX;

template<class T>
struct IsBulkSerializable
{
	static const bool value = true;
};

template<>
struct IsBulkSerializable<class SSObjMeshSection>
{
	static const bool value = false;
};

#pragma region SerializeReader
class GAMEMODULE_API SerializeReader
{
public:
	SerializeReader(const std::string& FilePath);

	friend SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT4& Vec4);
	friend SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT3& Vec3);
	friend SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT2& Vec2);
	friend SerializeReader& operator >> (SerializeReader& Archive, std::string& RHS);
	friend SerializeReader& operator >> (SerializeReader& Archive, float& Value);
	friend SerializeReader& operator >> (SerializeReader& Archive, unsigned int& Value);
	friend SerializeReader& operator >> (SerializeReader& Archive, size_t& Value);

	template<typename T>
	friend SerializeReader& operator >> (SerializeReader& Archive, std::vector<T>& RHS);

	template<typename T1, typename T2>
	friend SerializeReader& operator >> (SerializeReader& Archive, std::map<T1&, T2&>& RHS);

	bool IsGood() const;

	std::ifstream Stream;
};

SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT4& Vec4);
SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT3& Vec3);
SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT2& Vec2);
SerializeReader& operator >> (SerializeReader& Archive, std::string& RHS);
SerializeReader& operator >> (SerializeReader& Archive, size_t& Value);
SerializeReader& operator >> (SerializeReader& Archive, float& Value);
SerializeReader& operator >> (SerializeReader& Archive, unsigned int& Value);
SerializeReader& operator >> (SerializeReader& Archive, int& Value);
SerializeReader& operator >> (SerializeReader& Archive, bool& Value);
#ifdef THIS_IS_ENGINE_MODULE
#include "SSVertexTypes.h"
SerializeReader& operator >> (SerializeReader& Archive, VT_PositionNormalTexcoordTangent& Value);
SerializeReader& operator >> (SerializeReader& Archive, class SSObjMeshMaterial& Value);
#endif
#pragma endregion

class GAMEMODULE_API SerializeWriter
{
public:
	SerializeWriter(const std::string& FilePath);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT4& Vec4);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT3& Vec3);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT2& Vec2);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const std::string& RHS);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const float Value);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const unsigned int Value);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const int Value);
	friend SerializeWriter& operator << (SerializeWriter& Archive, const size_t& Value);

	friend SerializeWriter& operator << (SerializeWriter& Archive, const class SSObjMeshMaterial& Value);
	

	template<typename T>
	friend SerializeWriter& operator << (SerializeWriter& Archive, const std::vector<T>& RHS);

	template<typename T1, typename T2>
	friend SerializeWriter& operator << (SerializeWriter& Archive, const std::map<T1, T2>& RHS);

	bool IsGood() const;

	std::ofstream Stream;
};

//GAMEMODULE_API SerializeWriter& operator << (SerializeWriter& Archive, const class SSObjMeshMaterial& Value);

template<class T, bool bBulkSerialize>
class VectorSerializer
{
};


template<typename T>
class VectorSerializer<T, true>
{
public:
	SerializeWriter& WriteVector(SerializeWriter& Archive, const std::vector<T>& RHS)
	{
		auto Size = RHS.size();
		Archive.Stream.write((char*)&Size, sizeof(std::vector<T>::size_type));
		auto TotalBytes = RHS.size() * sizeof(T);
		Archive.Stream.write((char*)RHS.data(), TotalBytes);
		return Archive;
	}

	SerializeReader& ReadVector(SerializeReader& Archive, std::vector<T>& RHS)
	{
		std::vector<T>::size_type Size;
		Archive.Stream.read((char*)&Size, sizeof(std::vector<T>::size_type));
		RHS.resize(Size);
		auto TotalBytes = RHS.size() * sizeof(T);
		Archive.Stream.read((char*)RHS.data(), TotalBytes);
		return Archive;
	}
};

template<typename T>
class VectorSerializer<T, false>
{
public:
	SerializeWriter& WriteVector(SerializeWriter& Archive, const std::vector<T>& RHS)
	{
		auto Size = RHS.size();
		Archive.Stream.write((char*)&Size, sizeof(std::vector<T>::size_type));
		for (auto& Item : RHS)
		{
			Archive << Item;
		}
		return Archive;
	}

	SerializeReader& ReadVector(SerializeReader& Archive, std::vector<T>& RHS)
	{
		std::vector<T>::size_type Size;
		Archive.Stream.read((char*)&Size, sizeof(std::vector<T>::size_type));
		RHS.resize(Size);

		for (std::vector<T>::size_type i = 0; i < Size; ++i)
		{
			Archive >> RHS[i];
		}
		return Archive;
	}
};


template<typename T>
SerializeReader& operator >> (SerializeReader& Archive, std::vector<T>& RHS)
{
	VectorSerializer<T, IsBulkSerializable<T>::value> Serializer;

	Serializer.ReadVector(Archive, RHS);

	return Archive;
}

template<typename T>
SerializeWriter& operator<< (SerializeWriter& Archive, const std::vector<T>& RHS)
{
	VectorSerializer<T, IsBulkSerializable<T>::value> Serializer;

	Serializer.WriteVector(Archive, RHS);

	return Archive;
}

template<typename T1, typename T2>
SerializeWriter& operator << (SerializeWriter& Archive, const std::map<T1, T2>& RHS)
{
	UINT Size = static_cast<UINT>(RHS.size());

	Archive.Stream << Size;

	for (auto iter = RHS.begin(); iter != RHS.end(); ++iter)
	{
		Archive.Stream << iter->first;
		Archive.Stream << iter->second;
	}
}

template<typename T1, typename T2>
SerializeReader& operator >> (SerializeReader& Archive, std::map<T1&, T2&>& RHS)
{
	UINT Size;

	Archive.Stream >> Size;

	for(UINT i = 0; i < Size; ++i)
	{
		T1 key;
		T2 value;
		
		Archive.Stream >> key;
		Archive.Stream >> value;

		RHS[key] = value;
	}
}

SerializeWriter& operator << (SerializeWriter& Archive, const float Value);
SerializeWriter& operator << (SerializeWriter& Archive, const size_t& Value);
SerializeWriter& operator << (SerializeWriter& Archive, const unsigned int Value);
SerializeWriter& operator << (SerializeWriter& Archive, const int Value);
SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT4& Vec4);
SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT3& Vec3);
SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT2& Vec2);
SerializeWriter& operator << (SerializeWriter& Archive, const std::string& RHS);


std::ofstream& operator<<(std::ofstream& Archive, const std::string& S);
std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT2& Vec2);
std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT4& Vec4);
std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT3& Vec3);

#ifdef THIS_IS_ENGINE_MODULE
std::ofstream& operator<<(std::ofstream& Archive, const VT_PositionNormalTexcoord& p);
std::ofstream& operator<<(std::ofstream& Archive, const VT_PositionNormalTexcoordTangent& p);
#endif



template<typename T>
std::ofstream& operator<<(std::ofstream& Archive, const std::vector<T>& RHS)
{
	auto TotalBytes = RHS.size() * sizeof(T);

	Archive.write((const char*)RHS.data(), TotalBytes);

	return Archive;
}

template<typename T>
inline std::ifstream& operator>>(std::ifstream& Archive, std::vector<T>& RHS)
{
	auto TotalBytes = RHS.size() * sizeof(T);

	Archive.read((char*)RHS.data(), TotalBytes);

	return Archive;
}




