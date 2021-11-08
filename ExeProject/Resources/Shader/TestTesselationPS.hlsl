#include "TestTesselationShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(DSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(wrapPointSampler,input.uv);
	return texColor;
	//return float4(1.0f, 1.0f, 1.0f, 1.0f);
}