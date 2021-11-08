#include "TestTesselationShader.hlsli"

VSOutput main( VSInput input )
{
	VSOutput output;
	output.svpos = input.pos;
	output.wpos = input.pos;
	output.uv = input.uv;
	output.normal = input.normal;
	return output;
}