#include "SSGLTF.h"
#include "simdjson.h"

SSGLTF_V2 SSGLTF_V2::LoadGLTFFile(const std::string& InPath)
{
	simdjson::dom::parser Parser;
	simdjson::dom::element GLTFJson = Parser.load(InPath);

	for (auto Accessor : GLTFJson["accessors"].get_array())
	{
		auto JsonObject = Accessor.get_object();

		auto A = JsonObject["bufferView"].get_int64();
		auto B = JsonObject["componentType"].get_int64();
		auto C = JsonObject["count"].get_int64();	
		auto D = JsonObject["type"].get_string();		
	}

	for (auto BufferView : GLTFJson["bufferViews"].get_array())
	{
		auto JsonObject = 
	}
	return SSGLTF_V2{};
}