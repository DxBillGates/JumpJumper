#include "RGBShiftShader.hlsli"

Texture2D<float4> tex : register(t0);
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor;
	texColor.a = 1;
	texColor.r = tex.Sample(smp, input.uv + float2(shiftInfo.r, 0)).r;
	texColor.g = tex.Sample(smp, input.uv + float2(0,0)).g;
	texColor.b = tex.Sample(smp, input.uv + float2(-shiftInfo.r, 0)).b;
	return texColor;
}