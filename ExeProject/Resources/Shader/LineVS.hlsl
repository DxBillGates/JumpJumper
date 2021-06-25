#include "DefaultShader.hlsli"

LineVSOutput main(float4 pos : POSITION,float4 color : COLOR)
{
	LineVSOutput o;
	o.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, pos)));
	o.pos = pos;
	o.color = color;
	return o;
}