
struct VertexIn
{
    float4 position : POSITION;    
};

struct VertexOut
{
    float4 position : POSITION0;
    float4 worldPosition : POSITION1;
};


cbuffer Proj
{
	float4x4 ProjectionMatrix;	
};

cbuffer View
{
	float4x4 ViewMatrix;	
};


VertexOut VSMain(VertexIn vin)
{
	VertexOut output ;
	output.position = mul(vin.position, ViewMatrix);
	output.position = mul(output.position, ProjectionMatrix);

	output.worldPosition = vin.position;
    
    return output;
}