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
};

