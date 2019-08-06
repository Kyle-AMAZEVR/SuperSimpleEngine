

struct VertexIn
{
    float4 position : POSITION;       
    float3 color : COLOR;
    float2 texcoord : TEXCOORD;
};

struct VertexOut
{
    float4 position : SV_POSITION;
    float4 worldPosition : COLOR;
};

cbuffer Model
{
    float4x4 model; 
};


cbuffer Proj
{
    float4x4 proj;  
};

cbuffer View
{
    float4x4 view;  
};


VertexOut VSMain(VertexIn vin)
{
    VertexOut output ;
    float4x4 mvp = mul(mul(model, view), proj);
    output.position = mul(vin.position, mvp);

    output.worldPosition = vin.position;
    
    return output;
}