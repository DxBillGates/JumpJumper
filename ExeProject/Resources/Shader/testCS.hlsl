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
	returnData.x = frac(sin(dot(float2(0.016f * x, 0), float2(12.9898, 7.233))) * 43758.5453);
	returnData.y = frac(sin(dot(float2(0, 0.016f * x), float2(12.9898, 7.233))) * 43758.5453);
	returnData.z = frac(sin(dot(float2(0.016f * x, 0.016f * x), float2(12.9898, 7.233))) * 43758.5453);
	return returnData;
}

float3 GetRandomVectorPosBase(float3 pos)
{
	float3 returnData = float3(0, 0, 0);
	returnData.x = frac(sin(dot(float2(pos.x, 0), float2(12.9898, 7.233))) * 43758.5453);
	returnData.y = frac(sin(dot(float2(0, pos.y), float2(12.9898, 7.233))) * 43758.5453);
	returnData.z = frac(sin(dot(float2(pos.z, pos.z), float2(12.9898, 7.233))) * 43758.5453);
	return returnData;
}

RWStructuredBuffer<UAVData> real : register(u0);
StructuredBuffer<TestData> addVector : register(t0);

[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
	//�����̐ݒ�
	float maxSec = 100;

	//�����̉��Z
	real[DTid.x].vel.w += GetRandomVector(DTid.x).x;

	//�����_���x�N�g���̍쐬
	float4 randomVector = float4(0, 0, 0, 0);
	randomVector.xyz = GetRandomVector(DTid.x);
	randomVector = normalize(randomVector);

	////�^�[�Q�b�g�x�N�g���̌v�Z
	//float4 targetVec = addVector[0].pos - real[DTid.x].pos;
	//float3 test = targetVec.xyz;
	//targetVec = normalize(targetVec);

	////�ʒu�x�[�X�̃m�C�Y�x�N�g�� * sin���쐬
	//float4 randomSinVector = float4(0, 0, 0, 0);
	//randomSinVector.xyz = GetRandomVectorPosBase(real[DTid.x].pos.xyz);
	//randomSinVector.xyz = normalize(randomSinVector) * sin(real[DTid.x].vel.w);

	//�x�N�g���̉��Z
	real[DTid.x].vel.xyz += randomVector.xyz;
	real[DTid.x].vel.y -= real[DTid.x].vel.w * 0.016f;

	//float4 vec = targetVec;
	//real[DTid.x].vel += vec;

	//real[DTid.x].pos.x += real[DTid.x].vel.x + addVector[0].vel.x;
	//real[DTid.x].pos.y += real[DTid.x].vel.y + addVector[0].vel.y;
	//real[DTid.x].pos.z += real[DTid.x].vel.z + addVector[0].vel.z;

	real[DTid.x].pos.xyz += real[DTid.x].vel.xyz / 10;

	if (real[DTid.x].vel.w >= maxSec)
	{
		real[DTid.x].pos.xyz = addVector[0].pos.xyz;
		real[DTid.x].vel.xyz = float3(0, 0, 0);
		real[DTid.x].vel.w = 0;
	}

	//float dist = distance(addVector[0].pos.xyz, real[DTid.x].pos.xyz);
	//if (dist >= 1000)
	//{
	//	real[DTid.x].pos.xyz = addVector[0].pos.xyz;
	//	real[DTid.x].vel.xyz = float3(0, 0, 0);
	//}
}