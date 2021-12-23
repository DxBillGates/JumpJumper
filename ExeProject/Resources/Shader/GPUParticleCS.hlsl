// GPUParticleSystem����邽�߂̃e�X�g�V�F�[�_�[

// 1�u���b�N�������p�[�e�B�N���̐�
#define PARTICLE_PER_THREAD 256

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
	float3 pos;
	float3 vel;
	float alpha;
};

// �S�p�[�e�B�N���f�[�^
RWStructuredBuffer<ParticleData> particles : register(u0);

// ���f�[�^�i���Ƃ��d�͂Ƃ��j
StructuredBuffer<WorldData> worldData : register(t0);

// �S�G�~�b�^�[�f�[�^
StructuredBuffer<EmitterData> emitters : register(t1);


// �e��֐�
//--------------------------------------------------------------------------------------------------------------------------------//
void InitializeParticle(uint3 id)
{
	ParticleData data = particles.Load(id.x);
	EmitterData emitterData = emitters.Load(id.x);

	if (data.state == PARTICLE_STATE_INITIALIZE)
	{
		particles[id.x].state = PARTICLE_STATE_UPDATE;
		particles[id.x].isDead = 0;
		particles[id.x].life = 1;
		particles[id.x].scale = emitterData.initScale;
		particles[id.x].rotate = float3(0, 0, 0);
		particles[id.x].pos = emitterData.pos;
		particles[id.x].vel = emitterData.startForce;
		particles[id.x].alpha = 1;
	}
}

void UpdateParticle(uint3 id)
{
	ParticleData data = particles.Load(id.x);
	EmitterData emitterData = emitters.Load(id.x);

	if (data.state == PARTICLE_STATE_UPDATE)
	{
		if (particles[id.x].life <= 0)
		{
			particles[id.x].state = PARTICLE_STATE_DEAD;
		}
		particles[id.x].life -= PER_FRAME / emitterData.MAX_LIFE;
		//particles[id.x].scale = emitterData.initScale;
		//particles[id.x].rotate = float3(0, 0, 0);
		particles[id.x].pos = particles[id.x].vel;
		//particles[id.x].alpha = 1;
	}
	else if (data.state == PARTICLE_STATE_RAPID_DEAD)
	{
		if (particles[id.x].life <= 0)
		{
			particles[id.x].state = PARTICLE_STATE_DEAD;
		}
		particles[id.x].life -= PER_FRAME / emitterData.MAX_LIFE;
	}
}

void AddWorldForce(uint3 id)
{

}

void TerminateParticle(uint3 id)
{
	ParticleData data = particles.Load(id.x);
	EmitterData emitterData = emitters.Load(id.x);

	if (data.state == PARTICLE_STATE_DEAD)
	{
		particles[id.x].isDead = 1;
		particles[id.x].state = PARTICLE_STATE_INITIALIZE;
		particles[id.x].scale = float3(0, 0, 0);
		particles[id.x].rotate = float3(0, 0, 0);
		particles[id.x].pos = emitterData.pos;
		particles[id.x].vel = float3(0, 0, 0);
		particles[id.x].alpha = 0;
	}
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