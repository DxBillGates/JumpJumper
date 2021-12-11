#include "PointShader.hlsli"

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0,0),
	float4(-0.5f,0.5f,0 ,0),
	float4(0.5f,-0.5f,0 ,0),
	float4(0.5f,0.5f,0  ,0)
};

[maxvertexcount(vnum)]
void main(
	point GSInput input[1],
	inout TriangleStream< PSInput > output
)
{
	float3 addPos = posData[input[0].index].pos;
	//addPos.x = input[0].index;
	float3 addScale = 50 * posData[input[0].index].vel.w;
	float4x4 addModelMatrix =
	{
		addScale.x,0,0,addPos.x,
		0,addScale.y,0,addPos.y,
		0,0,addScale.z,addPos.z,
		0,0,0,1,
	};

	for (uint i = 0; i < vnum; i++)
	{
		PSInput element;
		
		element.svpos = input[0].svpos + offset_array[i];
		element.svpos = mul(projMatrix, mul(viewMatrix, mul(addModelMatrix,mul(billboard,element.svpos))));
		element.index = input[0].index;
		output.Append(element);
	}
}