cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer CameraInfo : register(b2)
{
	matrix viewMatrix;
	matrix projMatrix;
	float4 cameraPos;
}

struct PSInput
{
	float4 svpos : SV_POSITION;
	float4 pos : POSITION;
};

float3 GetPosition(matrix mat)
{
	return float3(-mat[0][3], -mat[1][3], -mat[2][3]);
}

struct ParticleData
{
	float4 pos;
	float4 vel;
};
StructuredBuffer<ParticleData> posData : register(t0);