#include "GaussBlurShader.hlsli"

Texture2D<float4> tex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	float4 result = 0;

	for (int i = 0; i < 16; ++i)
	{
		result += data[i].z * tex.Sample(clampPointSampler, input.uv + data[i].xy);
	}

	return float4(result.rgb,1);
}