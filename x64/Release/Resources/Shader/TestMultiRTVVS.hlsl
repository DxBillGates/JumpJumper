#include "TestMultiRTVShader.hlsli"

VSOutput main(VSInput input)
{
	VSOutput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, input.pos)));
	output.uv = input.uv;
	output.normal = mul(modelMatrix, float4(input.normal, 0)).xyz;
	//output.tpos = mul(projMatrix, mul(lightViewMatrix, mul(modelMatrix, input.pos)));
	output.tpos = mul(lightViewMatrix, mul(modelMatrix, input.pos));

	float r = 1;
	float4 minR = float4(-r, -r, -r, -r);
	float4 maxR = float4(r, r, r, r);
	output.tpos = clamp(output.tpos, minR, maxR);
	return output;
}