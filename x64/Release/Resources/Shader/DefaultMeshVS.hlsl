#include "DefaultMeshShader.hlsli"

DefaultMeshVSOutput main(DefaultMeshVSInput input)
{
	DefaultMeshVSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, input.pos)));
	output.uv = input.uv;
	output.normal = mul(modelMatrix, float4(input.normal, 0)).xyz;
	return output;
}