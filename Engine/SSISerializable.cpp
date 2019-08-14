
#include "Core.h"
#include "SSISerializable.h"
#include "DXVertexTypes.h"

#pragma region SerializeReader
SerializeReader::SerializeReader(const std::string& FilePath)
{
	Stream.open(FilePath.c_str(), std::ios::binary);

	check(Stream.good());
}

bool SerializeReader::IsGood() const
{
	return Stream.good();
}


SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT4& Vec4)
{
	Archive.Stream.read(reinterpret_cast<char*>(&Vec4.x), sizeof(XMFLOAT4));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT3& Vec3)
{
	Archive.Stream.read(reinterpret_cast<char*>(&Vec3.x), sizeof(XMFLOAT3));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, XMFLOAT2& Vec2)
{
	Archive.Stream.read(reinterpret_cast<char*>(&Vec2.x), sizeof(XMFLOAT2));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, std::string& RHS)
{
	std::string::size_type Size;

	Archive.Stream.read((char*)&Size, sizeof(std::string::size_type));

	RHS.resize(Size);

	auto TotalBytes = Size * sizeof(std::string::value_type);

	Archive.Stream.read((char*)RHS.data(), TotalBytes);

	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, size_t& Value)
{
	Archive.Stream.read((char*)&Value, sizeof(size_t));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, VT_PositionNormalTexcoordTangent& Value)
{
	Archive >> Value.VertexAttribute1 >> Value.VertexAttribute2 >> Value.VertexAttribute3 >> Value.VertexAttribute4;
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, float& Value)
{
	Archive.Stream.read((char*)&Value, sizeof(float));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, unsigned int& Value)
{
	Archive.Stream.read((char*)&Value, sizeof(unsigned int));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, int& Value)
{
	Archive.Stream.read((char*)&Value, sizeof(int));
	return Archive;
}

SerializeReader& operator >> (SerializeReader& Archive, bool& Value)
{
	Archive.Stream.read((char*)&Value, sizeof(bool));
	return Archive;
}



#pragma endregion

SerializeWriter::SerializeWriter(const std::string& FilePath)
{
	Stream.open(FilePath.c_str(), std::ios::binary | std::ios::out);

	check(Stream.good());
}


bool SerializeWriter::IsGood() const
{
	return Stream.good();
}


SerializeWriter& operator << (SerializeWriter& Archive, const float Value)
{
	Archive.Stream.write((char*)&Value, sizeof(float));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const size_t& Value)
{
	Archive.Stream.write((char*)&Value, sizeof(size_t));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const unsigned int Value)
{
	Archive.Stream.write((char*)&Value, sizeof(unsigned int));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const int Value)
{
	Archive.Stream.write((char*)&Value, sizeof(int));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT4& Vec4)
{
	Archive.Stream.write((char*)(&Vec4.x), sizeof(XMFLOAT4));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT3& Vec3)
{
	Archive.Stream.write((char*)(&Vec3.x), sizeof(XMFLOAT3));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const XMFLOAT2& Vec2)
{
	Archive.Stream.write((char*)(&Vec2.x), sizeof(XMFLOAT2));
	return Archive;
}

SerializeWriter& operator << (SerializeWriter& Archive, const std::string& RHS)
{
	auto Size = RHS.size();

	Archive.Stream.write((char*)&Size, sizeof(std::string::size_type));

	auto TotalBytes = Size * sizeof(std::string::value_type);

	Archive.Stream.write((char*)RHS.data(), TotalBytes);

	return Archive;
}

std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT4& Vec4)
{
	Archive << Vec4.x << Vec4.y << Vec4.z << Vec4.w;

	return Archive;
}

std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT3& Vec3)
{
	Archive << Vec3.x << Vec3.y << Vec3.z;
	return Archive;
}

std::ofstream& operator<<(std::ofstream& Archive, const XMFLOAT2& Vec2)
{
	Archive << Vec2.x << Vec2.y;
	return Archive;
}



std::ofstream& operator<<(std::ofstream& Archive, const std::string& S)
{
	Archive << S.length();
	Archive.write(S.data(), S.length());
	return Archive;
}


std::ofstream& operator<< (std::ofstream& Archive, const VT_PositionNormalTexcoordTangent& p)
{
	Archive << p.VertexAttribute1 << p.VertexAttribute2 << p.VertexAttribute3 << p.VertexAttribute4;
	return Archive;
}

std::ofstream& operator<<(std::ofstream& Archive, const VT_PositionNormalTexcoord& p)
{
	Archive << p.VertexAttribute1 << p.VertexAttribute2 << p.VertexAttribute3;
	return Archive;
}

std::ifstream& operator>>(std::ifstream& Archive, XMFLOAT4& Vec4)
{
	Archive >> Vec4.x >> Vec4.y >> Vec4.z >> Vec4.w;
	return Archive;
}




inline std::ifstream& operator >> (std::ifstream& Archive, XMFLOAT3& Vec3)
{
	Archive >> Vec3.x >> Vec3.y >> Vec3.z;
	return Archive;
}



inline std::ifstream& operator >> (std::ifstream& Archive, XMFLOAT2& Vec2)
{
	Archive >> Vec2.x >> Vec2.y;
	return Archive;
}

std::ifstream& operator >> (std::ifstream& Archive, std::string& S)
{
	std::string::size_type Size;
	Archive >> Size;
	S.resize(Size);
	Archive.read(&S[0], Size);
	return Archive;
}

