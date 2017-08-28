#include "MeshLayout.hlsli"

VertexOutput main(VertexInput IN)
{
	VertexOutput ret;

	ret.position = float4(0, 0, 0, 0);
	ret.normal = float3(0, 0, 0);
	ret.color = float4(0, 0, 0, 0);

	return ret;
}