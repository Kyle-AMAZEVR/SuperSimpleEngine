



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
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float3 color : COLOR;    
};


PixelInputType VSMain(VertexInputType input)
{
    PixelInputType output;

    float4x4 mvp = mul(mul(model, view), proj);

    output.position = mul(input.position, mvp);    
    
    output.color = input.color;    
    
    return output;
}
