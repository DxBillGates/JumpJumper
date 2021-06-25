#include "DefaultShader.hlsli"

Texture2D<float4> tex : register(t0);
Texture2D<float> depthTex : register(t1);
SamplerState smp : register(s0);

float4 main(DefaultVSOutput vsOutput) : SV_TARGET
{
	float4 texColor = tex.Sample(smp,vsOutput.uv);
	//float dep = pow(depthTex.Sample(smp,vsOutput.uv),20);
	//dep = 1 - dep;
	return texColor;
	//return float4(dep,dep,dep,1);
}