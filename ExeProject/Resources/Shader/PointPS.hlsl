#include "PointShader.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampPointSampler, input.uv);
	//return float4(1,1,1, 1);
	return float4(texColor.rgba);
}