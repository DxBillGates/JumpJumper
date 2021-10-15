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

RWStructuredBuffer<UAVData> real : register(u0);
StructuredBuffer<TestData> addVector : register(t0);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//float4 v = real[DTid.x].vel;
	//float3 k = float3(10, 100, 10);
	//real[DTid.x].vel.y += y / k.y;

	//ターゲットベクトルの計算
	float4 targetVec = addVector[0].pos - real[DTid.x].pos;
	float3 test = targetVec.xyz;
	targetVec = normalize(targetVec);

	float4 vec = targetVec;
	real[DTid.x].vel += vec;

	real[DTid.x].pos.x += real[DTid.x].vel.x;
	real[DTid.x].pos.y += real[DTid.x].vel.y;
	real[DTid.x].pos.z += real[DTid.x].vel.z;


}