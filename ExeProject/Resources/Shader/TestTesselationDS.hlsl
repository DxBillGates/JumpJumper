#include "TestTesselationShader.hlsli"

#define NUM_CONTROL_POINTS 4

[domain("quad")]
DSOutput main(
	HS_CONSTANT_DATA_OUTPUT input,
	float2 domain : SV_DomainLocation,
	const OutputPatch<HS_CONTROL_POINT_OUTPUT, NUM_CONTROL_POINTS> patch)
{
	DSOutput output;

	//float4 pos = patch[0].wpos * domain.x + patch[1].wpos * domain.y + patch[2].wpos * domain.x;
	//float2 uv = patch[0].uv * domain.x + patch[1].uv * domain.y + patch[2].uv * domain.x;
	float3 normal = float3(0,0,-1);

	float3 p01 = lerp(patch[0].wpos, patch[1].wpos, domain.x);
	float3 p23 = lerp(patch[2].wpos, patch[3].wpos, domain.x);
	float3 pos = lerp(p01, p23, domain.y);

	float2 uv01 = lerp(patch[0].uv, patch[1].uv, domain.x);
	float2 uv23 = lerp(patch[2].uv, patch[3].uv, domain.x);
	float2 uv = lerp(uv01, uv23, domain.y);

	float4 height = heightMapTex.SampleLevel(wrapLinearSampler, uv,0);
	pos.z = -height * 10;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, float4(pos.xyz,1))));
	output.uv = uv;
	output.normal = mul(modelMatrix, float4(normal, 0)).xyz;
	output.color = height;


	return output;
}
