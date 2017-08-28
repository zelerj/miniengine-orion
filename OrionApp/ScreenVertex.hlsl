#include "ScreenLayout.hlsli"

VertexOutput main(VertexInput IN)
{
	VertexOutput ret;

	ret.position = float4(IN.vertex, 0);

	return ret;
}