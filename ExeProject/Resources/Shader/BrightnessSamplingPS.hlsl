#include "BrightnessSamplingShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> subTex : register(t1);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampLinearSampler,input.uv);
	//float4 subTexColor = subTex.Sample(clampPointSampler, input.uv);
	//texColor *= subTexColor;
	float greyScale = (texColor.r * 0.299f + texColor.g * 0.587f + texColor.b * 0.114f);
	float3 stepValue = step(samplingValue.r, texColor.rgb * greyScale);
	return float4(stepValue * texColor.rgb,1);
}