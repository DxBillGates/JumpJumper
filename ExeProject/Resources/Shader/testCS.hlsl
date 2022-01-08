// GPUParticleSystemを作るためのテストシェーダー

// ランダム関数
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

// ランダム関数範囲指定版
float3 GetRandomVector(float3 vec, float3 minValue, float3 maxValue)
{
	float3 result = float3(0, 0, 0);
	result = GetRandomVector(vec);
	result = (result / 1.0f * maxValue) - (-minValue) / 2.0f;
	return result;
}


// 1ブロックが扱うパーティクルの数
#define PARTICLE_PER_THREAD 128

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
	float deltaTime;
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
	float3 vel;

	// 追加データ
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

// 全パーティクルデータ
RWStructuredBuffer<ParticleData> particles : register(u0);

// 全エミッターデータ
StructuredBuffer<EmitterData> emitters : register(t0);

// 環境データ（風とか重力とか）
StructuredBuffer<WorldData> worldData : register(t1);


// 各種関数
//--------------------------------------------------------------------------------------------------------------------------------//
void InitializeParticle(uint3 id)
{
	if (particles[id.x].state != PARTICLE_STATE_INITIALIZE)return;

	// エミッターから情報を取得して初期化＆ランダムな加速度を付与
	particles[id.x].depth = 0;
	particles[id.x].isDead = 0;
	particles[id.x].life = 1;
	particles[id.x].vel.w = 1;

	particles[id.x].state = PARTICLE_STATE_UPDATE;

	//particles[id.x].scale = emitters[particles[id.x].emitter].initScale;
	//particles[id.x].rotate = float3(0, 0, 0);
	particles[id.x].pos = float4(emitters[0].pos,0);
	//particles[id.x].alpha = emitters[particles[id.x].emitter].initAlpha;

	//ランダムベクトルの作成
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

	// ライフが0以下なら死亡フラグを立ててライフ関係を初期化
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

	// 生きているなら所属エミッターから付与されるべき力を加算＆位置更新
	float4 addVec = float4(emitters[0].pos,0) - particles[id.x].pos;
	particles[id.x].vel.xyz += emitters[0].vel.xyz;
	UpdatePosition(id);
	// scaleやrotateなどの追加アイテムの更新


	// ライフをデルタタイム分減らす
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

	// データの初期化
	particles[id.x].depth = 0;
	particles[id.x].state = PARTICLE_STATE_NONE;

	particles[id.x].scale = float3(0, 0, 0);
	particles[id.x].rotate = float3(0, 0, 0);
	particles[id.x].pos = float4(emitters[0].pos,0);
	particles[id.x].vel = float4(0, 0, 0,0);
	particles[id.x].alpha = 0;
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
