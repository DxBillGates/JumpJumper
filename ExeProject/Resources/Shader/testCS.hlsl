//struct UAVData
//{
//	// �ǂ̃G�~�b�^�[�ɑ����Ă��邩
//	int emitter;
//	// �[�x�l�i�\�[�g�p�j
//	float depth;
//
//	// ���
//	int state;
//	// �����t���O
//	int isDead;
//	// ���C�t
//	float life;
//
//	// ��b�f�[�^
//	float3 scale;
//	float3 rotate;
//	float4 pos;
//	float4 vel;
//	float alpha;
//};
//
//struct TestData
//{
//	// �ǂ̃G�~�b�^�[�ɑ����Ă��邩
//	int emitter;
//	// �[�x�l�i�\�[�g�p�j
//	float depth;
//
//	// ���
//	int state;
//	// �����t���O
//	int isDead;
//	// ���C�t
//	float life;
//
//	// ��b�f�[�^
//	float3 scale;
//	float3 rotate;
//	float4 pos;
//	float4 vel;
//	float alpha;
//};
//
//float3 GetRandomVector(float x)
//{
//	float3 returnData = float3(0, 0, 0);
//	returnData.x = frac(sin(dot(float2(0.016f * x, 0), float2(12.9898, 78.233))) * 43758.5453);
//	returnData.y = frac(sin(dot(float2(0, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
//	returnData.z = frac(sin(dot(float2(0.016f * x, 0.016f * x), float2(12.9898, 78.233))) * 43758.5453);
//	return returnData;
//}
//
//float3 GetRandomVector(float3 vec)
//{
//	float3 result = float3(0,0,0);
//	result.x = frac(sin(dot(vec.zy + vec.x, float2(12.9898, 78.233))) * 43758.5453);
//	result.y = frac(sin(dot(vec.yz + vec.x, float2(12.9898, 78.233))) * 43758.5453);
//	result.z = frac(sin(dot(vec.xy + vec.z, float2(12.9898, 78.233))) * 43758.5453);
//	return result;
//}
//
//float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
//{
//	float3 result = float3(0, 0, 0);
//	result = GetRandomVector(vec);
//	result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
//	return result;
//}
//
//float GetRandom(float2 vec)
//{
//	float result = 0;
//	result = frac(sin(dot(vec, float2(12.9898, 78.233))) * 43758.5453);
//	return result;
//}
//
//RWStructuredBuffer<UAVData> real : register(u0);
//StructuredBuffer<TestData> addVector : register(t0);
//
//[numthreads(128, 1, 1)]
//void main(uint3 DTid : SV_DispatchThreadID)
//{
//	//�����̐ݒ�
//	float maxSec = 1;
//
//	//�����̉��Z
//	float frame = 1.0f / 60.0f;
//	float minR = frame / 10;
//	float maxR = frame;
//
//	float3 random = GetRandomVector(GetRandomVector(DTid.x), float3(minR, minR, minR), float3(maxR, maxR, maxR));
//	real[DTid.x].vel.w -= (random.x + random.y + random.z) / 3.0f;
//
//	//�����_���x�N�g���̍쐬
//	float range = 1;
//	float4 randomVector = float4(0, 0, 0, 0);
//	randomVector.xyz = GetRandomVector(GetRandomVector(DTid.x),float3(-range, -range, -range),float3(range, range, range));
//	randomVector = normalize(randomVector)/10;
//
//	////�^�[�Q�b�g�x�N�g���̌v�Z
//	//float4 targetVec = addVector[0].pos - real[DTid.x].pos;
//	//float3 test = targetVec.xyz;
//	//targetVec = normalize(targetVec);
//
//	////�ʒu�x�[�X�̃m�C�Y�x�N�g�� * sin���쐬
//	//float4 randomSinVector = float4(0, 0, 0, 0);
//	//randomSinVector.xyz = GetRandomVectorPosBase(real[DTid.x].pos.xyz);
//	//randomSinVector.xyz = normalize(randomSinVector) * sin(real[DTid.x].vel.w);
//
//	//�x�N�g���̉��Z
//	real[DTid.x].vel.xyz += randomVector.xyz;
//	//real[DTid.x].vel.y -= real[DTid.x].vel.w * 0.016f;
//
//	//float4 vec = targetVec;
//	//real[DTid.x].vel += vec;
//
//	//real[DTid.x].pos.x += real[DTid.x].vel.x + addVector[0].vel.x;
//	//real[DTid.x].pos.y += real[DTid.x].vel.y + addVector[0].vel.y;
//	//real[DTid.x].pos.z += real[DTid.x].vel.z + addVector[0].vel.z;
//
//	real[DTid.x].pos.xyz += real[DTid.x].vel.xyz / 10;
//
//	if (real[DTid.x].vel.w < 0)
//	{
//		real[DTid.x].pos.xyz = addVector[0].pos.xyz;
//		real[DTid.x].vel.xyz = float3(0, 0, 0);
//		real[DTid.x].vel.w = maxSec;
//	}
//
//	//float dist = distance(addVector[0].pos.xyz, real[DTid.x].pos.xyz);
//	//if (dist >= 1000)
//	//{
//	//	real[DTid.x].pos.xyz = addVector[0].pos.xyz;
//	//	real[DTid.x].vel.xyz = float3(0, 0, 0);
//	//}
//
//	//for (int i = 0; i < PARTICLE_PER_THREAD; i++)
//	//{
//	//	int globalId = PARTICLE_PER_THREAD * DTid.x + i;
//
//	//	//real[globalId].pos.x = globalId;
//	//	//real[globalId].pos.y = 0;
//	//	//real[globalId].pos.z = 0;
//
//	//	//real[globalId].pos.xyz += addVector[0].pos.xyz;
//	//	////�����̐ݒ�
//	//	float maxSec = 1;
//
//	//	//�����̉��Z
//	//	float frame = 1.0f / 60.0f;
//	//	float minR = frame / 10;
//	//	float maxR = frame;
//
//	//	float3 random = GetRandomVector(GetRandomVector(globalId), float3(minR, minR, minR), float3(maxR, maxR, maxR));
//	//	real[globalId].vel.w -= (random.x + random.y + random.z) / 3.0f;
//
//	//	//�����_���x�N�g���̍쐬
//	//	float range = 1;
//	//	float4 randomVector = float4(0, 0, 0, 0);
//	//	randomVector.xyz = GetRandomVector(GetRandomVector(globalId), float3(-range, -range, -range), float3(range, range, range));
//	//	randomVector = normalize(randomVector) / 10;
//
//	//	//�x�N�g���̉��Z
//	//	real[globalId].vel.xyz += randomVector.xyz;
//
//	//	real[globalId].pos.xyz += real[globalId].vel.xyz / 10;
//
//	//	if (real[globalId].vel.w < 0)
//	//	{
//	//		real[globalId].pos.xyz = float3(0, 0, 0);
//	//		real[globalId].vel.xyz = float3(0, 0, 0);
//	//		real[globalId].vel.w = maxSec;
//	//	}
//	//}
//
//	UAVData data = real[DTid.x];
//	data.pos = float4(0, 0, 0, 0);
//}
//

// GPUParticleSystem����邽�߂̃e�X�g�V�F�[�_�[

// �����_���֐�
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

// �����_���֐��͈͎w���
float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
{
	float3 result = float3(0, 0, 0);
	result = GetRandomVector(vec);
	result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
	return result;
}


// 1�u���b�N�������p�[�e�B�N���̐�
#define PARTICLE_PER_THREAD 128

// �A�v���P�[�V�������g����p�[�e�B�N���̑���
#define MAX_PARTICLE 256000

// �A�v���P�[�V�������g����G�~�b�^�[�̑���
#define MAX_EMITTER 1000

#define PER_FRAME 1.0f / 60.0f

struct WorldData
{
	float3 gravityForce;
	float3 windForce;
	float3 force;
	float deltaTime;
};

#define EMITTER_STATE_DONTUSE -1
#define EMITTER_STATE_START 0
#define EMITTER_STATE_END 1
struct EmitterData
{
	// �G�~�b�^�[�̏��
	int state;

	// �G�~�b�^�[�̃��[�J�����W
	float3 pos;

	// �G�~�b�^�[������o�����Ƃ��̃x�N�g��
	float3 startForce;

	// �G�~�b�^�[�������Ă����
	float3 addForce;

	// �ǉ��f�[�^
	float MAX_LIFE;
	float3 initScale;
	float initAlpha;
};

#define PARTICLE_STATE_RAPID_DEAD -1
#define PARTICLE_STATE_INITIALIZE 0
#define PARTICLE_STATE_UPDATE 1
#define PARTICLE_STATE_DEAD 2
#define PARTICLE_STATE_NONE 3
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

// �S�p�[�e�B�N���f�[�^
RWStructuredBuffer<ParticleData> particles : register(u0);

// �S�G�~�b�^�[�f�[�^
StructuredBuffer<ParticleData> emitters : register(t0);

// ���f�[�^�i���Ƃ��d�͂Ƃ��j
StructuredBuffer<WorldData> worldData : register(t1);


// �e��֐�
//--------------------------------------------------------------------------------------------------------------------------------//
void InitializeParticle(uint3 id)
{
	if (particles[id.x].state != PARTICLE_STATE_INITIALIZE)return;

	// �G�~�b�^�[��������擾���ď������������_���ȉ����x��t�^
	particles[id.x].depth = 0;
	particles[id.x].isDead = 0;
	particles[id.x].life = 1;
	particles[id.x].vel.w = 1;

	particles[id.x].state = PARTICLE_STATE_UPDATE;

	//particles[id.x].scale = emitters[particles[id.x].emitter].initScale;
	//particles[id.x].rotate = float3(0, 0, 0);
	particles[id.x].pos = emitters[0].pos;
	//particles[id.x].alpha = emitters[particles[id.x].emitter].initAlpha;

	//�����_���x�N�g���̍쐬
	float range = 1;
	float4 randomVector = float4(0, 0, 0, 0);
	randomVector.xyz = GetRandomVector(GetRandomVector(id.x), float3(-range, -range, -range), float3(range, range, range));
	randomVector = normalize(randomVector) * 100;
	particles[id.x].vel.xyz = randomVector.xyz;
}

void UpdatePosition(uint3 id)
{
	if (particles[id.x].state == PARTICLE_STATE_UPDATE)
	{
		particles[id.x].pos += particles[id.x].vel * PER_FRAME;
	}
	else
	{
		particles[id.x].pos = emitters[0].pos;
	}
}

void UpdateParticle(uint3 id)
{
	if (particles[id.x].state != PARTICLE_STATE_UPDATE)return;

	// ���C�t��0�ȉ��Ȃ玀�S�t���O�𗧂Ăă��C�t�֌W��������
	//if (particles[id.x].life <= 0)
	//{
	//	particles[id.x].state = PARTICLE_STATE_DEAD;
	//	particles[id.x].life = 0;
	//	particles[id.x].isDead = 1;
	//}

	if (particles[id.x].vel.w <= 0)
	{
		particles[id.x].state = PARTICLE_STATE_DEAD;
		particles[id.x].vel.w = 0;
		particles[id.x].life = 0;
		particles[id.x].isDead = 1;
	}

	// �����Ă���Ȃ珊���G�~�b�^�[����t�^�����ׂ��͂����Z���ʒu�X�V
	particles[id.x].vel += emitters[0].vel;
	UpdatePosition(id);
	// scale��rotate�Ȃǂ̒ǉ��A�C�e���̍X�V


	// ���C�t���f���^�^�C�������炷
	particles[id.x].vel.w -= PER_FRAME;
	//particles[id.x].life -= worldData.deltaTime;
}

void AddWorldForce(uint3 id)
{
	if (particles[id.x].state != PARTICLE_STATE_UPDATE)return;
}

void TerminateParticle(uint3 id)
{
	if (particles[id.x].state != PARTICLE_STATE_DEAD)return;

	// �f�[�^�̏�����
	particles[id.x].depth = 0;
	particles[id.x].state = PARTICLE_STATE_NONE;

	particles[id.x].scale = float3(0, 0, 0);
	particles[id.x].rotate = float3(0, 0, 0);
	particles[id.x].pos = emitters[0].pos;
	particles[id.x].vel = float4(0, 0, 0,0);
	particles[id.x].alpha = 0;
}

//--------------------------------------------------------------------------------------------------------------------------------//


// �R���s���[�g�V�F�[�_�[�̃��C���֐�
//--------------------------------------------------------------------------------------------------------------------------------//
[numthreads(PARTICLE_PER_THREAD, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	// ���̃t���[������V���Ɏg����p�[�e�B�N����������
	InitializeParticle(id);
	// �p�[�e�B�N�����g���ێ����Ă��鑬�x�A�G�~�b�^�[�����͂Ȃǂňʒu��T�C�Y�Ȃǂ��X�V
	UpdateParticle(id);
	// �d�͂╗�̉e����t�^
	AddWorldForce(id);
	// ���̃t���[���Ŏ��ʃp�[�e�B�N����������
	TerminateParticle(id);
}
//--------------------------------------------------------------------------------------------------------------------------------//
