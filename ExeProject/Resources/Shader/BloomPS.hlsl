#include "BloomShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> brightnessGaussTex : register(t1);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampLinearSampler,input.uv);
	float4 bloomColor = brightnessGaussTex.Sample(clampLinearSampler, input.uv);
	return float4((texColor.rgb + bloomColor.rgb), 1.0f);
}