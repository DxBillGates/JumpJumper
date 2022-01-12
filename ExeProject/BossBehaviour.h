#pragma once
#include "Header/Component/Behaviour.h"
#include "GameState.h"
#include "Header/Math/Vector3.h"

enum class BossState
{
	NONE,
	JOIN,
	HEALING,
	LEFT,
};

class BossBehaviour : public GatesEngine::Behaviour
{
private:
private:
	BossState state;
	bool isJoining;
	float joiningTime;
	bool isLefting;
	float leftingTime;
	GatesEngine::Math::Vector3 preLerpPos;

	bool stopFlag;
	float stopingTime;

	const float MAX_HP;
	float hp;

	float oldHP;
	bool decreaseHpTimeFlag;
	float decreaseHpTime;
	const float MAX_DECREASE_HP_TIME;

	bool isDead;
	float scale;
	float initScale;
private:
	void InitState();
	void JoinOrLeft(BossState state);
	void Stoping();
public:
	BossBehaviour();
	~BossBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::Collider* otherCollider);
	void SetBossState(BossState state);
	BossState GetState();
	float GetHp();
	bool GetIsEndScaleAnimation();
	void SetInitScale(float value);
	bool GetIsDead();
};

