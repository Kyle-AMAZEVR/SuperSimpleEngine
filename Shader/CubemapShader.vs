

struct VertexIn
{
    float3 PosL : POSITION;
};

struct VertexOut
{
    float4 PosH : SV_POSITION;
    float3 PosL : POSITION;
};
 

 cbuffer MVP
{
    float4x4 WorldViewProj;
};
 

VertexOut VSMain(VertexIn vin)
{
    VertexOut vout;
    
    // Set z = w so that z/w = 1 (i.e., skydome always on far plane).
    vout.PosH = mul(float4(vin.PosL, 1.0f), WorldViewProj).xyww;
    
    // Use local vertex position as cubemap lookup vector.
    vout.PosL = vin.PosL;
    
    return vout;
}