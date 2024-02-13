#include "SSGLTF.h"
#include "simdjson.h"

namespace GLTF {
	SSGLTF_V2 SSGLTF_V2::LoadGLTFFile(const std::string& InPath)
	{
		simdjson::dom::parser Parser;
		simdjson::dom::element GLTFJson = Parser.load(InPath);

		SSGLTF_V2 Result{};

		for (auto Element : GLTFJson["accessors"].get_array())
		{
			auto JsonObject = Element.get_object();
			auto TypeString = JsonObject["type"].get_string();

			Accessor AccessorObject{};
			AccessorObject.BufferView = static_cast<int>(JsonObject["bufferView"].get_int64());
			AccessorObject.ComponentType = static_cast<ComponentType>(JsonObject["componentType"].get_int64().take_value());
			AccessorObject.Count = static_cast<int>(JsonObject["count"].get_int64());

			Result.Accessors.push_back(AccessorObject);
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



		return Result;
	}
}
