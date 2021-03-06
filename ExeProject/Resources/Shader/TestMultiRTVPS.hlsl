#include "TestMultiRTVShader.hlsli"

Texture2D<float4> lightDepthTex : register(t0);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);

PSOutput main(VSOutput input)
{
	PSOutput output;

	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(input.normal);
	float intensity = saturate(dot(-lightDir, normal));
	float4 color = float4(intensity, intensity, intensity, 1) * lightColor;
	//////?ȈՃg?D?[??
	//if (intensity > 0.95)
	//	color = float4(1, 1, 1, 1.0);
	//else if (intensity > 0.2)
	//	color = float4(0.7, 0.7, 0.7, 1.0);
	//else if (intensity > 0.1)
	//	color = float4(0.5, 0.5, 0.5, 1.0);
	//else
	//	color = float4(0.2, 0.2, 0.2, 1.0);

	output.result = color;

	float shadowWeight = 1;
	float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
	float2 shadowUV = saturate((posFromLightVP.xy + float2(1, -1)) * float2(0.5, -0.5));
	float4 depthFromLight = lightDepthTex.Sample(clampPointSampler, shadowUV);
	shadowWeight = (depthFromLight.r < posFromLightVP.z - 0.0001f) ? 0.1f : 1;
	output.shadow = float4(shadowWeight, shadowWeight, shadowWeight, 1);
	return output;
}