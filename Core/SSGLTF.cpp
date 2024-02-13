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
			AccessorObject.BufferView = static_cast<int>(JsonObject["bufferView"].get_int64().take_value());
			AccessorObject.ComponentType = static_cast<ComponentType>(JsonObject["componentType"].get_int64().take_value());
			AccessorObject.Count = static_cast<int>(JsonObject["count"].get_int64().take_value());
			AccessorObject.Name = JsonObject["name"].get_string().take_value();

			Result.Accessors.push_back(AccessorObject);
		}

		for (auto Element : GLTFJson["bufferViews"].get_array())
		{
			auto JsonObject = Element.get_object();

			BufferView View{};
			View.Buffer = JsonObject["buffer"].get_int64().take_value();
			View.ByteLength = JsonObject["byteLength"].get_int64().take_value();
			View.ByteOffset =  JsonObject["byteOffset"].get_int64().take_value();
			View.Target = JsonObject["target"].get_int64().take_value();
			View.Name = JsonObject["name"].get_string().take_value();
		}

		for (auto Image : GLTFJson["images"].get_array())
		{
			auto JsonObject = Image.get_object();

			auto Uri = JsonObject["uri"];
		}



		return Result;
	}
}
