#include "BloomShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> brightnessGaussTex : register(t1);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp,input.uv);
	float4 bloomColor = brightnessGaussTex.Sample(smp, input.uv);
	return float4((texColor.rgb + bloomColor.rgb), 1.0f);
}