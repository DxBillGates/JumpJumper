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
	// どのエミッターに属しているか
	int emitter;
	// 深度値（ソート用）
	float depth;

	// 状態
	int state;
	// 生存フラグ
	int isDead;
	// ライフ
	float life;

	// 基礎データ
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