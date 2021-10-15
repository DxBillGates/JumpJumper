#include "PointShader.hlsli"

PSInput main(float4 pos : POSITION, uint index : SV_InstanceID)
{
	PSInput output;
	output.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, pos + posData[index].pos)));
	output.pos = posData[index].pos;
	//float4 svpos;
	//svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, pos + posData[index])));
	return output;
}