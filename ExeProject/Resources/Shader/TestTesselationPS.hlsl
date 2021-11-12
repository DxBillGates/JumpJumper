#include "TestTesselationShader.hlsli"

float4 main(DSOutput input) : SV_TARGET
{
	float4 texColor = tex.SampleLevel(wrapPointSampler,input.uv,0);
	return texColor;
	return float4(input.color, 1.0f);
}