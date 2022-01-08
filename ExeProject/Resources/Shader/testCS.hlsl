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
	float3 vel;

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
StructuredBuffer<EmitterData> emitters : register(t0);

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
	particles[id.x].pos = float4(emitters[0].pos,0);
	//particles[id.x].alpha = emitters[particles[id.x].emitter].initAlpha;

	//�����_���x�N�g���̍쐬
	float range = 1;
	float4 randomVector = float4(0, 0, 0, 0);
	randomVector.xyz = GetRandomVector(GetRandomVector(id.x), float3(-range, -range, -range), float3(range, range, range));
	randomVector = normalize(randomVector) * 100;
	particles[id.x].vel.xyz = randomVector.xyz + emitters[0].startForce;
}

void UpdatePosition(uint3 id)
{
	if (particles[id.x].state == PARTICLE_STATE_UPDATE)
	{
		particles[id.x].pos += particles[id.x].vel * PER_FRAME;
	}
	else
	{
		particles[id.x].pos = float4(emitters[0].pos, 0);
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
	float4 addVec = float4(emitters[0].pos,0) - particles[id.x].pos;
	particles[id.x].vel.xyz += emitters[0].vel.xyz;
	UpdatePosition(id);
	// scale��rotate�Ȃǂ̒ǉ��A�C�e���̍X�V


	// ���C�t���f���^�^�C�������炷
	particles[id.x].vel.w -= PER_FRAME / emitters[0].MAX_LIFE;
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
	particles[id.x].pos = float4(emitters[0].pos,0);
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
