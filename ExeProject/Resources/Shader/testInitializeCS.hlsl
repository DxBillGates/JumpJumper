struct UAVData
{
	float4 pos;
	float4 vel;
};

struct TestData
{
	float4 pos;
	float4 vel;
};

float3 GetRandomVector(float x)
{
	float3 returnData = float3(0, 0, 0);
	returnData.x = frac(sin(dot(float2(0.016f * x, 0), float2(12.9898, 78.233))) * 43758.5453);
	returnData.y = frac(sin(dot(float2(0, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
	returnData.z = frac(sin(dot(float2(0.016f * x, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
	return returnData;
}

float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
{
	float3 result = float3(0, 0, 0);
	result = GetRandomVector(vec);
	result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
	return result;
}

RWStructuredBuffer<UAVData> real : register(u0);
StructuredBuffer<TestData> addVector : register(t0);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	if (real[DTid.x].vel.w < 0)
	{
		real[DTid.x].pos.xyz = addVector[0].pos.xyz;
		real[DTid.x].vel.xyz = float3(0, 0, 0);
		real[DTid.x].vel.w = 0;
	}

	//Žõ–½‚ÌÝ’è
	float maxSec = 1;

	//Žõ–½‚Ì‰ÁŽZ
	float frame = 1.0f / 60.0f;
	float minR = frame / 10;
	float maxR = frame;

	float3 random = GetRandomVector(GetRandomVector(DTid.x), float3(minR, minR, minR), float3(maxR, maxR, maxR));
	real[DTid.x].vel.w -= (random.x + random.y + random.z) / 3.0f;

	real[DTid.x].pos.xyz += real[DTid.x].vel.xyz / 10;

	if (real[DTid.x].vel.w < 0)
	{
		real[DTid.x].pos.xyz = addVector[0].pos.xyz;
		real[DTid.x].vel.xyz = float3(0, 0, 0);
		real[DTid.x].vel.w = 0;
	}
}