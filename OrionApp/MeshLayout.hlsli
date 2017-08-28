#define LAYOUT_MESH

// Uniforms
cbuffer MatricesCBuffer : register(b0)
{
	matrix Model;
	matrix View;
	matrix Projection;
}

// Data Layout
struct VertexInput
{
	float3 vertex : POSITION;
	float3 normal : NORMAL;
	float3 tangent : TANGENT;
	float4 color : COLOR;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
	float3 normal : NORMAL0;
	float4 color : COLOR0;
};