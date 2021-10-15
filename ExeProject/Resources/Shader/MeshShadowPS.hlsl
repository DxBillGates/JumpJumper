#include "MeshShadowShader.hlsli"

Texture2D<float4> lightDepthTex : register(t0);
SamplerState smp : register(s0);

float4 main(PSInput input) : SV_TARGET
{
	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(input.normal);
	float intensity = saturate(dot(-lightDir, normal));
	float4 result = float4(intensity, intensity, intensity, 1) * lightColor;

	float shadowWeight = 1;
	if (input.tpos.x < 1 && input.tpos.x > -1 && input.tpos.y < 1 && input.tpos.y > -1 && input.tpos.z < 1 && input.tpos.z > -1)
	{
		float3 posFromLightVP = input.tpos.xyz / input.tpos.w;
		float2 shadowUV = saturate((posFromLightVP.xy + float2(1, -1)) * float2(0.5, -0.5));
		float depthFromLight = lightDepthTex.Sample(smp, shadowUV);
		shadowWeight = (depthFromLight < posFromLightVP.z - 0.001f) ? 0.5f : 1;
	}

	return float4(result.rgb * shadowWeight,1);
}