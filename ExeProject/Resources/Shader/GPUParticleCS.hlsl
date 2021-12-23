// GPUParticleSystemを作るためのテストシェーダー

// 1ブロックが扱うパーティクルの数
#define PARTICLE_PER_THREAD 256

// アプリケーションが使えるパーティクルの総量
#define MAX_PARTICLE 256000

// アプリケーションが使えるエミッターの総量
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
	// エミッターの状態
	int state;

	// エミッターのローカル座標
	float3 pos;

	// エミッターから放出されるときのベクトル
	float3 startForce;

	// エミッターが放っている力
	float3 addForce;

	// 追加データ
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
	float3 pos;
	float3 vel;
	float alpha;
};

// 全パーティクルデータ
RWStructuredBuffer<ParticleData> particles : register(u0);

// 環境データ（風とか重力とか）
StructuredBuffer<WorldData> worldData : register(t0);

// 全エミッターデータ
StructuredBuffer<EmitterData> emitters : register(t1);


// 各種関数
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


// コンピュートシェーダーのメイン関数
//--------------------------------------------------------------------------------------------------------------------------------//
[numthreads(PARTICLE_PER_THREAD, 1, 1)]
void main(uint3 id : SV_DispatchThreadID)
{
	// 今のフレームから新たに使われるパーティクルを初期化
	InitializeParticle(id);
	// パーティクル自身が保持している速度、エミッターが放つ力などで位置やサイズなどを更新
	UpdateParticle(id);
	// 重力や風の影響を付与
	AddWorldForce(id);
	// 今のフレームで死ぬパーティクルを初期化
	TerminateParticle(id);
}
//--------------------------------------------------------------------------------------------------------------------------------//