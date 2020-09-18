

struct VertexIn
{
    float4 PosL : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float4 PosL : POSITION;
    float3 normal : NORMAL;
    float2 texcoord : TEXCOORD;
    float4 tangent : TANGENT;
};
 

cbuffer MVP
{
    float4x4 WorldViewProj;
};


 

VertexOut VSMain(VertexIn vin)
{
    VertexOut vout;
    
    // Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(vin.PosL, WorldViewProj).xyww;
    
    // Use local vertex position as cubemap lookup vector.
    vout.PosL = vin.PosL;

    vout.normal = vin.normal;
    vout.texcoord = vin.texcoord;
    vout.tangent = vin.tangent;
    
    return vout;
}
