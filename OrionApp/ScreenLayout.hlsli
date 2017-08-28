#define LAYOUT_SCREEN

// Uniforms

// Data Layout
struct VertexInput
{
	float3 vertex : POSITION;
};

struct VertexOutput
{
	float4 position : SV_POSITION;
};