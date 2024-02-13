#include "SSGLTF.h"
#include "simdjson.h"

SSGLTF_V2 SSGLTF_V2::LoadGLTFFile(const std::string& InPath)
{
	simdjson::dom::parser Parser;
	simdjson::dom::element GltfJson = Parser.load(InPath);

	return SSGLTF_V2{};
}