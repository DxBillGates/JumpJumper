#include "DefaultShader.hlsli"

cbuffer Time : register(b4)
{
	// cpu deltaTime
	float4 time;
}

Texture2D<float4> tex : register(t0);
//Texture2D<float> depthTex : register(t1);
SamplerState smp : register(s0);

float4 main(DefaultVSOutput vsOutput) : SV_TARGET
{
	float2 perPixel = float2(1.0f /1920.0f,1.0f/1080.0f);
	float4 texColor = tex.Sample(smp,vsOutput.uv + time.xy);
	//float dep = pow(depthTex.Sample(smp,vsOutput.uv),20);
	//dep = 1 - dep;
	return texColor;
	//return float4(dep,dep,dep,1);
}