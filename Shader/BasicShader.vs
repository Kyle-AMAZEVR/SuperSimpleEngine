

cbuffer MVP : register(b0)
{
    float4x4 ModelViewProj;
};

struct VertexInputType
{
    float4 position : POSITION;
    float3 color : COLOR;    
    float2 texcoord : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 color : COLOR;    
    float2 texcoord : TEXCOORD;
};

PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, ModelViewProj);
    
    // Store the input color for the pixel shader to use.    
    output.color = input.color;
    output.texcoord = input.texcoord;
    
    return output;
}
