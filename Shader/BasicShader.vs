





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
    
    
    // Change the position vector to be 4 units for proper matrix calculations.
    input.position.w = 1.0f;

    // Calculate the position of the vertex against the world, view, and projection matrices.
    output.position = mul(input.position, model);
    output.position = mul(output.position, view);
    output.position = mul(output.position, proj);
    
    // Store the texture coordinates for the pixel shader.
    output.texcoord = input.texcoord;
    
    // Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)model);
    
    // Normalize the normal vector.
    output.normal = normalize(output.normal);
    
    return output;
}
