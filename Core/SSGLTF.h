#pragma once

namespace GLTF {
    enum CORE_API ComponentType
    {
        BYTE = 5120,
        UNSIGNED_BYTE = 5121,
        SHORT = 5122,
        UNSIGNED_SHORT = 5123,
        UNSIGNED_INT = 5125,
        FLOAT = 5126,
    };

    enum CORE_API AttributeType
    {
        /// SCALAR
        /// VEC2
        /// VEC3
        /// VEC4
        /// MAT2
        /// MAT3
        /// MAT4
        None,
        SCALAR,
        VEC2,
        VEC3,
        VEC4,
        MAT2,
        MAT3,
        MAT4,
    };
   
    enum CORE_API BufferTarget
    {
        //
        ARRAY_BUFFER = 34962,
        ELEMENT_ARRAY_BUFFER = 34963,
    };

    enum CORE_API AlphaMode
    {
        OPAQUE_MODE,
        MASK_MODE,
        BLEND_MODE,
    };

    class CORE_API Buffer
    {
    public:
        int ByteLength;
        int Type;
        int Uri;
    };

    class CORE_API BufferView
    {
    public:
        int Buffer;
        int ByteLength;
        int ByteOffset;
        int Target;
        std::string Name;
        int ByteStride;
    };


    class CORE_API MinMax
    {

    };

    class CORE_API Attributes
    {
    public:
        int Normal;
        int Position;
        int TexCoord0;
    };

    class CORE_API Accessor
    {
    public:
        int BufferView;
        int ByteOffset;
        GLTF::AttributeType Type;
        int Count;
        GLTF::ComponentType ComponentType;


    };

    class CORE_API Primitive
    {
    public:
        int Mode;
        std::map<std::string, int> Attributes;
        int Indices;
        int Material;
    };

    class CORE_API EmissiveTexture 
    {
    public:
        int Index;
    };

    class CORE_API NormalTexture
    {
    public:
        int Index;
    };

    class CORE_API OcclusionTexture
    {
    public:
        int Index;
    };

    class CORE_API BaseColorTexture 
    {
    public:
        int Index;
    };

    class CORE_API MetallicRoughnessTexture
    {
    public:
        int Index;
    };

    class CORE_API Material
    {
    public:
        
    };

    class CORE_API Mesh
    {
    public:
        std::string Name;
        std::vector<Primitive> Primitives;
    };

    class CORE_API SSGLTF_V2
    {
    public:

        static SSGLTF_V2 LoadGLTFFile(const std::string& InPath);

    protected:
        std::vector<std::string> Images;
    };
};

