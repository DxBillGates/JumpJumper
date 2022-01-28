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

enum class BossMoveState
{
	NONE,

	// ���`�ړ�
	LERP,

	// ��]�ړ�
	ROTATE,
};

enum class BossAttackState
{
	NONE,

	// �U������
	PRE,
	
	// �U��
	ATTACK,

	// �U���J�n
	END,
};

enum class BossAttackMode
{
	NONE,

	// �ːi�U��
	CHARGE_ATTACK,
};

class BossBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::GameObject* target;

	BossState state;
	BossMoveState moveState;
	BossAttackState attackState;
	BossAttackMode attackMode;
private:
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

	bool chargeFlag;
	bool chargeAttackFlag;
	GatesEngine::Math::Vector3 chargeAttackVector;
	GatesEngine::Math::Vector3 startChargeAttackPos;
	float chargeAttackTime;
	float chargeTime;
	int chargeAttackDrawCount;
	int chargeAttackFrameCount;
private:
	void PreAttack();
	void Attack();
	void EndAttack();
	void SetState();
	void SetAttackMode(BossAttackMode setAttackMode);
	void SetMoveState(BossMoveState setMoveState);
	float CalcTargetDistance();
private:
	void InitState();
	void JoinOrLeft(BossState state);
	void Stoping();
	void PreChargeAttack();
	void ChargeAttack();
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
	void SetTarget(GatesEngine::GameObject* targetObject);
};

