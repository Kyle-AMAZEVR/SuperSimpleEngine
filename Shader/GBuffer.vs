
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
    float2 TexCoord : TEXCOORD0;
    float4 Tangent : TANGENT;
};


struct PixelInputType
{
    float4 OutPosition : SV_POSITION;
    float4 OutViewPosition : COLOR0;
    float2 OutTexCoord : TEXCOORD0;
    float3 OutNormal : NORMALWS;
    float3 OutTangent : TANGENTWS;
    float3 OutBitangent : BITANGENTWS;
};


  
PixelInputType VSMain( VertexInputType vin ) 
{	
	
    PixelInputType output;

	float4x4 ModelView = mul(model, view);
	float4x4 MVP = mul(mul(model, view), proj);

	output.OutTexCoord = vin.TexCoord;
	output.OutPosition = mul(vin.VertexPosition, MVP);
	output.OutViewPosition = mul(vin.VertexPosition, ModelView);
	
	float3 normalWS = normalize(mul(vin.VertexNormal, (float3x3)ModelView));
	
	output.OutNormal = normalWS;
	
	float3 tangentWS = normalize(mul(vin.Tangent.xyz, (float3x3)ModelView));
	
	output.OutTangent = tangentWS;

	float3 bitangentWS = normalize(cross(normalWS, tangentWS)) * vin.Tangent.w;	
	
	output.OutBitangent = bitangentWS;

	return output;	
}