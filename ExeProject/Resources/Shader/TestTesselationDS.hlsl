#include "TestTesselationShader.hlsli"

#define NUM_CONTROL_POINTS 3

[domain("tri")]
DSOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float3 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DSOutput output;

	float4 pos = patch[0].wpos * domain.x + patch[1].wpos * domain.y + patch[2].wpos * domain.z;
	float2 uv = patch[0].uv * domain.x + patch[1].uv * domain.y + patch[2].uv * domain.z;
	float3 normal = patch[0].normal * domain.x + patch[1].normal * domain.y + patch[2].normal * domain.z;

	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, float4(pos.xyz,1))));
	output.uv = uv;
	output.normal = mul(modelMatrix, float4(normal, 0)).xyz;


	return output;
}
