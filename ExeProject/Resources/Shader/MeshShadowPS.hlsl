#include "MeshShadowShader.hlsli"

Texture2D<float4> lightDepthTex : register(t0);
SamplerState smp : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(input.normal);
	float intensity = saturate(dot(-lightDir, normal));
	float4 color = float4(intensity, intensity, intensity, 1);
	if (intensity > 0.95)
		color = float4(1.0, 1, 1, 1.0);
	else if (intensity > 0.5)
		color = float4(0.7, 0.7, 0.7, 1.0);
	else if (intensity > 0.05)
		color = float4(0.35, 0.35, 0.35, 1.0);
	else
		color = float4(0.1, 0.1, 0.1, 1.0);

	float4 result = float4(color.xyz, 1) * lightColor;

	float shadowWeight = 1;
	if (input.tpos.x < 1 && input.tpos.x > -1 && input.tpos.y < 1 && input.tpos.y > -1 && input.tpos.z < 1 && input.tpos.z > -1)
	{
		float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
		float2 shadowUV = saturate((posFromLightVP.xy + float2(1, -1)) * float2(0.5, -0.5));
		float4 depthFromLight = lightDepthTex.Sample(smp, shadowUV);
		shadowWeight = (depthFromLight.r < posFromLightVP.z - 0.001f) ? 0.5f : 1;
	}

	return float4(result.rgb * shadowWeight,1);
}