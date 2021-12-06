#include "PointShader.hlsli"

GSInput main(float4 pos : POSITION, uint index : SV_InstanceID)
{
	GSInput output;
	output.svpos = pos;
	output.index = index;
	//output.pos = posData[index].pos;
	//output.alpha = posData[index].vel.w;
	//float4 svpos;
	//svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, pos + posData[index])));
	return output;
}