#include "ScreenLayout.hlsli"

// Add multiple RT for deferred
float4 main(VertexOutput IN) : SV_TARGET
{
	//return float4(IN.position.x, IN.position.y, 0.0f, 1.0f);
	return float4(1, 1, 1, 1);
}