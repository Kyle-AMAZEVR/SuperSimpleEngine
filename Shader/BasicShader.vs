

cbuffer Transform
{
    matrix Model;
    matrix View;
    matrix Projection;
};

struct VertexInputType
{
    float4 position : POSITION;
    float3 normal : NORMAL;    
    float2 texcoord : TEXCOORD;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 normal : NORMAL;    
    float2 texcoord : TEXCOORD;
};

PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, Model);
    output.position = mul(output.position, View);
    output.position = mul(output.position, Projection);    
    
    // Store the input color for the pixel shader to use.
    output.normal = input.normal;
    output.texcoord = input.texcoord;
    
    return output;
}
