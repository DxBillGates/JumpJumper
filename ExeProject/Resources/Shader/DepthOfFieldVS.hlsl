#include "DepthOfFieldShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.svpos = mul(projMatrix, mul(modelMatrix, input.pos));
	output.uv = input.uv;
	return output;
}