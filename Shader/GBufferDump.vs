struct VertexInputType
{
    float4 position : POSITION;
    float2 texcoord : TEXCOORD0;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 texcoord : TEXCOORD0;
};

PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = input.position;

    // Store the input color for the pixel shader to use.
    output.texcoord = input.texcoord;
    
    return output;
}
