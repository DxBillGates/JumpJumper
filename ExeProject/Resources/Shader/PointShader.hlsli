cbuffer Model : register(b0)
{
	matrix modelMatrix;
}

cbuffer CameraInfo : register(b2)
{
	matrix viewMatrix;
	matrix projMatrix;
	float4 cameraPos;
	matrix billboard;
}

struct GSInput
{
	float4 svpos : SV_POSITION;
	uint index : TEXCOORD;
};

struct PSInput
{
	float4 svpos : SV_POSITION;
	uint index : TEXCOORD;
	float2 uv : TEXCOORD1;
};

float3 GetPosition(matrix mat)
{
	return float3(-mat[0][3], -mat[1][3], -mat[2][3]);
}

struct ParticleData
{
	// �ǂ̃G�~�b�^�[�ɑ����Ă��邩
	int emitter;
	// �[�x�l�i�\�[�g�p�j
	float depth;

	// ���
	int state;
	// �����t���O
	int isDead;
	// ���C�t
	float life;

	// ��b�f�[�^
	float3 scale;
	float3 rotate;
	float4 pos;
	float4 vel;
	float alpha;
};
StructuredBuffer<ParticleData> posData : register(t0);
Texture2D<float4> tex : register(t1);

SamplerState wrapPointSampler : register(s0);
SamplerState clampPointSampler : register(s1);
SamplerState wrapLinearSampler  : register(s2);
SamplerState clampLinearSampler : register(s3);