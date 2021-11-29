#include "TestMultiRTVShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, input.pos)));
	output.uv = input.uv;
	output.normal = mul(modelMatrix, float4(input.normal, 0)).xyz;
	//output.tpos = mul(projMatrix, mul(lightViewMatrix, mul(modelMatrix, input.pos)));
	output.tpos = mul(lightViewMatrix, mul(modelMatrix, input.pos));
	return output;
}