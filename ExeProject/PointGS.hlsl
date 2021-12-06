#include "PointShader.hlsli"

struct GSOutput
{
	float4 pos : SV_POSITION;
};

static const uint vnum = 4;

static const float4 offset_array[vnum] =
{
	float4(-0.5f,-0.5f,0),
	float4(-0.5f,0.5f,0),
	float4(0.5f,-0.5f,0),
	float4(0.5f,0.5f,0)
};

[maxvertexcount(vnum)]
void main(
	point float4 input[1] : SV_POSITION, 
	inout TriangleStream< PSInput > output
)
{
	for (uint i = 0; i < vnum; i++)
	{
		PSInput element;
		element.svpos = input[0] + offset_array[i];
		element.svpos = mul(projMatrix, mul(viewMatrix, mul(modelMatrix, element.svpos + posData[index].pos)));
		output.Append(element);
	}
}