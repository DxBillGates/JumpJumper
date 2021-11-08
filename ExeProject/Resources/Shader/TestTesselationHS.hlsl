#include "TestTesselationShader.hlsli"

#define NUM_CONTROL_POINTS 3

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT Output;

	float3 retf;
	float ritf, uitf;

	ProcessTriTessFactorsAvg(tesselationInfo.x, 1, retf, ritf, uitf);

	Output.EdgeTessFactor[0] = retf.x;
	Output.EdgeTessFactor[1] = retf.y;
	Output.EdgeTessFactor[2] = retf.z;
	Output.InsideTessFactor = ritf;;

	return Output;
}

[domain("tri")]
[partitioning("integer")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(3)]
[patchconstantfunc("CalcHSPatchConstants")]
HS_CONTROL_POINT_OUTPUT main( 
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip, 
	uint i : SV_OutputControlPointID )
{
	HS_CONTROL_POINT_OUTPUT output;

	output.svpos = ip[i].svpos;
	output.wpos = ip[i].wpos;
	output.uv = ip[i].uv;
	output.normal = ip[i].normal;


	return output;
}
