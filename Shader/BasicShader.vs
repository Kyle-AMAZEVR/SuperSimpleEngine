




cbuffer TestColor
{
    float4 givencolor;  
};

cbuffer Model
{
  float4x4 model;  
};

cbuffer View
{
  float4x4 view; 
};

cbuffer Proj
{
  float4x4 proj;
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
    
    //output.position = mul(input.position, mvp);

    float4x4 mvp = mul(mul(model, view), proj);

    output.position = mul(input.position, mvp);
    
    // Store the input color for the pixel shader to use.    
    output.color = input.color;
    output.texcoord = input.texcoord;
    
    return output;
}
