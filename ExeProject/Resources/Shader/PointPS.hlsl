#include "PointShader.hlsli"

float4 main(PSInput input) : SV_TARGET
{
	return float4(normalize(input.pos.xyz), 1.0f);
}