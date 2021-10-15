#include "DefaultShader.hlsli"

float4 main(LineVSOutput i) : SV_TARGET
{
	return i.color + lightColor;
}