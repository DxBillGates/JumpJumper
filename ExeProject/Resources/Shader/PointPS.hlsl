#include "PointShader.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	return float4(0,0,0, 1);
	//return float4(0,0,0, posData[input.index].vel.w);
}