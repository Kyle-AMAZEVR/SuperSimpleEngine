#include "SSGLTF.h"
#include "simdjson.h"

namespace GLTF {
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
			auto JsonObject = BufferView.get_object();

			auto A = JsonObject["buffer"].get_int64();
			auto B = JsonObject["byteLength"].get_int64();
			auto C = JsonObject["byteOffset"].get_int64();
			auto D = JsonObject["target"].get_int64();
		}

		for (auto Image : GLTFJson["images"].get_array())
		{
			auto JsonObject = Image.get_object();

			auto Uri = JsonObject["uri"];
		}



		return SSGLTF_V2{};
	}
}
