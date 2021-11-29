#include "DefaultShader.hlsli"

cbuffer Time : register(b4)
{
	// cpu deltaTime
	float4 time;
}

Texture2D<float4> tex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(DefaultVSOutput vsOutput) : SV_TARGET
{
	float2 perPixel = float2(1.0f /1920.0f,1.0f/1080.0f);
	float4 texColor = tex.Sample(wrapPointSampler,vsOutput.uv + time.xy);
	return texColor;
}