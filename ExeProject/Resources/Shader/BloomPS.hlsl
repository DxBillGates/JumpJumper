#include "BloomShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> brightnessGaussTex : register(t1);
Texture2D<float4> brightnessGaussTex2 : register(t2);
Texture2D<float4> brightnessGaussTex3 : register(t3);


SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(clampLinearSampler,input.uv);
	float4 bloomColor = brightnessGaussTex.Sample(clampLinearSampler, input.uv);
	float4 bloomColor2 = brightnessGaussTex2.Sample(clampLinearSampler, input.uv);
	float4 bloomColor3 = brightnessGaussTex3.Sample(clampLinearSampler, input.uv);
	float4 bloomResult = bloomColor.rgba + bloomColor2.rgba + bloomColor3.rgba;
	//bloomResult /= 3;
	//return float4(texColor.rgb + bloomColor.rgb, 1.0f);
	//return float4(texColor.rgb + bloomColor2.rgb, 1.0f);
	//return float4(texColor.rgb + bloomColor3.rgb, 1.0f);
	return float4((texColor.rgb + bloomResult.rgb), 1.0f);
}