
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
    float4 VertexPosition : POSITION;
    float3 VertexNormal  : NORMAL;
    float2 TexCoord : TEXCOORD;
    float4 Tangent : TANGENT;
};


struct PixelInputType
{
    float4 OutPosition : SV_POSITION;
    float4 OutViewPosition : COLOR;
    float2 OutTexCoord : TEXCOORD0;
    float3 OutNormal : NORMAL;
    float3 OutTangent : TANGENT;
    float3 OutBinormal : BINORMAL;
};


  
PixelInputType VSMain( VertexInputType vin ) 
{	
	
    PixelInputType output;

	float4x4 ModelView = mul(model, view);
	float4x4 MVP = mul(ModelView, proj);

	output.OutTexCoord = vin.TexCoord;
	output.OutPosition = mul(vin.VertexPosition, MVP);
	output.OutViewPosition = mul(vin.VertexPosition, ModelView);
	output.OutNormal = normalize(mul(float4(vin.VertexNormal,0), ModelView)).xyz;
	output.OutTangent = normalize(mul(vin.Tangent, ModelView)).xyz;

	float3 binormal = (cross(vin.VertexNormal, vin.Tangent.xyz)) * -vin.Tangent.w;
	output.OutBinormal = normalize(mul( float4(binormal,0), ModelView)).xyz;

	return output;	
}