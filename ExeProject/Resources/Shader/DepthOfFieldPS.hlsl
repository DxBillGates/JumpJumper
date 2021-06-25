#include "DepthOfFieldShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float4> gaussTex : register(t1);
Texture2D<float4> depthTex : register(t2);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp,input.uv);
	float4 gaussTexColor = gaussTex.Sample(smp, input.uv);
	float4 depthTexColor = depthTex.Sample(smp, input.uv);

	float t = saturate((pow(abs(depthTexColor.r),20) - dofValue.r) * 3);

	float4 result = lerp(texColor,gaussTexColor,t);
	return result;
}