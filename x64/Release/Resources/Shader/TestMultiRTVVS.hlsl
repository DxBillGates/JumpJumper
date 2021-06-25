#include "TestMultiRTVShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.svpos = mul(projMatrix,mul(modelMatrix, input.pos));
	output.uv = input.uv;
	output.normal = input.normal;
	return output;
}