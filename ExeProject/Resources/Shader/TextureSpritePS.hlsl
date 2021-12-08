#include "TextureSpriteShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> subtex : register(t1);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampPointSampler,input.uv);
	//texColor += subtex.Sample(clampPointSampler, input.uv);
	return texColor;
}