#include "TestTesselationShader.hlsli"

//距離ベーステッセレーション係数
float CalcTessFactor(float4 vertex)
{
	//ワールド座標からカメラの座標を引く
	float dist = length(mul(vertex, modelMatrix).xyz - cameraPos.xyz);
	float tessNear = factor.x;
	float tessFar = factor.y;
	const float maxTessFactor = 64.0f;//最大分割数
	float var = maxTessFactor - (maxTessFactor - 1) * (dist - tessNear) / (tessFar - tessNear);
	return max(1, min(maxTessFactor, var));
}

//視線ベクトルと法線
float CalcNormalBias(float3 p, float3 normal)
{
	const float normalThershold = 0.85f;
	float3 fromCamera = normalize(p - cameraPos.xyz);
	float cos2 = dot(normal, fromCamera);
	cos2 *= cos2;
	float normalFactor = 1.0f - cos2;
	float bias = dot(normalFactor - normalThershold, 0) / (1.0f - normalThershold);
	return bias * clamp(factor.z, 0, 64.0f);
}

#define NUM_CONTROL_POINTS 4

HS_CONSTANT_DATA_OUTPUT CalcHSPatchConstants(
	InputPatch<VSOutput, NUM_CONTROL_POINTS> ip,
	uint PatchID : SV_PrimitiveID)
{
	HS_CONSTANT_DATA_OUTPUT output;

	float4 v[4];
	float3 normal = float3(0, 0, -1);
	int indices[4][2] = { int2(0, 1), int2(1, 2), int2(2, 3), int2(0, 3) };

	for (int i = 0; i < 4; i++)
	{
		int index0 = indices[i][0];
		int index1 = indices[i][1];
		v[i] = 0.5f * (ip[index0].wpos + ip[index1].wpos);
	}

	output.EdgeTessFactor[0] = CalcTessFactor(v[0]);
	output.EdgeTessFactor[0] += CalcNormalBias(v[0].xyz, normal);
	output.EdgeTessFactor[1] = CalcTessFactor(v[1]);
	output.EdgeTessFactor[1] += CalcNormalBias(v[1].xyz, normal);
	output.EdgeTessFactor[2] = CalcTessFactor(v[2]);
	output.EdgeTessFactor[2] += CalcNormalBias(v[2].xyz, normal);
	output.EdgeTessFactor[3] = CalcTessFactor(v[3]);
	output.EdgeTessFactor[3] += CalcNormalBias(v[3].xyz, normal);

	//output.EdgeTessFactor[0] = output.EdgeTessFactor[1] = output.EdgeTessFactor[2] = output.EdgeTessFactor[3] = 10;
	//output.InsideTessFactor[0] = output.InsideTessFactor[1] = 10;
	output.InsideTessFactor[0] = 0.5f * (output.EdgeTessFactor[0] + output.EdgeTessFactor[2]);
	output.InsideTessFactor[1] = 0.5f * (output.EdgeTessFactor[1] + output.EdgeTessFactor[3]);
	return output;

	//float3 retf;
	//float ritf, uitf;

	//ProcessTriTessFactorsAvg(tesselationInfo.x, 1, retf, ritf, uitf);

	//Output.EdgeTessFactor[0] = retf.x;
	//Output.EdgeTessFactor[1] = retf.y;
	//Output.EdgeTessFactor[2] = retf.z;
	//Output.EdgeTessFactor[3] = retf.z;
	//Output.InsideTessFactor[0] = ritf;
	//Output.InsideTessFactor[1] = ritf;;

	//return Output;
}

[domain("quad")]
[partitioning("fractional_even")]
[outputtopology("triangle_cw")]
[outputcontrolpoints(4)]
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
