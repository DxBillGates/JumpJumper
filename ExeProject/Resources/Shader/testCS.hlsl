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

float3 GetRandomVector(float3 vec)
{
	float3 result = float3(0,0,0);
	result.x = frac(sin(dot(vec.zy + vec.x, float2(12.9898, 78.233))) * 43758.5453);
	result.y = frac(sin(dot(vec.yz + vec.x, float2(12.9898, 78.233))) * 43758.5453);
	result.z = frac(sin(dot(vec.xy + vec.z, float2(12.9898, 78.233))) * 43758.5453);
	return result;
}

float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
{
	float3 result = float3(0, 0, 0);
	result = GetRandomVector(vec);
	result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
	return result;
}

float GetRandom(float2 vec)
{
	float result = 0;
	result = frac(sin(dot(vec, float2(12.9898, 78.233))) * 43758.5453);
	return result;
}

RWStructuredBuffer<UAVData> real : register(u0);
StructuredBuffer<TestData> addVector : register(t0);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//寿命の設定
	float maxSec = 1;

	//寿命の加算
	float frame = 1.0f / 60.0f;
	float minR = frame / 10;
	float maxR = frame;

	float3 random = GetRandomVector(GetRandomVector(DTid.x), float3(minR, minR, minR), float3(maxR, maxR, maxR));
	real[DTid.x].vel.w -= (random.x + random.y + random.z) / 3.0f;

	//ランダムベクトルの作成
	float range = 1;
	float4 randomVector = float4(0, 0, 0, 0);
	randomVector.xyz = GetRandomVector(GetRandomVector(DTid.x),float3(-range, -range, -range),float3(range, range, range));
	randomVector = normalize(randomVector)/10;

	////ターゲットベクトルの計算
	//float4 targetVec = addVector[0].pos - real[DTid.x].pos;
	//float3 test = targetVec.xyz;
	//targetVec = normalize(targetVec);

	////位置ベースのノイズベクトル * sinを作成
	//float4 randomSinVector = float4(0, 0, 0, 0);
	//randomSinVector.xyz = GetRandomVectorPosBase(real[DTid.x].pos.xyz);
	//randomSinVector.xyz = normalize(randomSinVector) * sin(real[DTid.x].vel.w);

	//ベクトルの加算
	real[DTid.x].vel.xyz += randomVector.xyz;
	//real[DTid.x].vel.y -= real[DTid.x].vel.w * 0.016f;

	//float4 vec = targetVec;
	//real[DTid.x].vel += vec;

	//real[DTid.x].pos.x += real[DTid.x].vel.x + addVector[0].vel.x;
	//real[DTid.x].pos.y += real[DTid.x].vel.y + addVector[0].vel.y;
	//real[DTid.x].pos.z += real[DTid.x].vel.z + addVector[0].vel.z;

	real[DTid.x].pos.xyz += real[DTid.x].vel.xyz / 10;

	if (real[DTid.x].vel.w < 0)
	{
		real[DTid.x].pos.xyz = addVector[0].pos.xyz;
		real[DTid.x].vel.xyz = float3(0, 0, 0);
		real[DTid.x].vel.w = maxSec;
	}

	//float dist = distance(addVector[0].pos.xyz, real[DTid.x].pos.xyz);
	//if (dist >= 1000)
	//{
	//	real[DTid.x].pos.xyz = addVector[0].pos.xyz;
	//	real[DTid.x].vel.xyz = float3(0, 0, 0);
	//}

	//for (int i = 0; i < PARTICLE_PER_THREAD; i++)
	//{
	//	int globalId = PARTICLE_PER_THREAD * DTid.x + i;

	//	//real[globalId].pos.x = globalId;
	//	//real[globalId].pos.y = 0;
	//	//real[globalId].pos.z = 0;

	//	//real[globalId].pos.xyz += addVector[0].pos.xyz;
	//	////寿命の設定
	//	float maxSec = 1;

	//	//寿命の加算
	//	float frame = 1.0f / 60.0f;
	//	float minR = frame / 10;
	//	float maxR = frame;

	//	float3 random = GetRandomVector(GetRandomVector(globalId), float3(minR, minR, minR), float3(maxR, maxR, maxR));
	//	real[globalId].vel.w -= (random.x + random.y + random.z) / 3.0f;

	//	//ランダムベクトルの作成
	//	float range = 1;
	//	float4 randomVector = float4(0, 0, 0, 0);
	//	randomVector.xyz = GetRandomVector(GetRandomVector(globalId), float3(-range, -range, -range), float3(range, range, range));
	//	randomVector = normalize(randomVector) / 10;

	//	//ベクトルの加算
	//	real[globalId].vel.xyz += randomVector.xyz;

	//	real[globalId].pos.xyz += real[globalId].vel.xyz / 10;

	//	if (real[globalId].vel.w < 0)
	//	{
	//		real[globalId].pos.xyz = float3(0, 0, 0);
	//		real[globalId].vel.xyz = float3(0, 0, 0);
	//		real[globalId].vel.w = maxSec;
	//	}
	//}
}

