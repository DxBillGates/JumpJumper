#include "DefaultMeshShader.hlsli"

float4 main(DefaultMeshVSOutput input) : SV_TARGET
{
	float3 lightDir = normalize(worldLightDir.xyz);
	float3 normal = normalize(input.normal);
	float intensity = saturate(dot(-lightDir, normal));
	float4 result = float4(intensity, intensity, intensity, 1) * lightColor;
	return result;
}