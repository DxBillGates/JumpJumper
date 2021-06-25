#include "DefaultShader.hlsli"

DefaultVSOutput main(DefaultVSInput vsInput)
{
	DefaultVSOutput vsOutput;
	vsOutput.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, vsInput.pos)));
	//vsOutput.svpos = vsInput.pos;
	vsOutput.uv = vsInput.uv;
	vsOutput.normal = mul(modelMatrix, float4(vsInput.normal, 0)).xyz;

	return vsOutput;
}