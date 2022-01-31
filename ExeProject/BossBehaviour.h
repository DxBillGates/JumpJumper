#pragma once
#include "Header/Component/Behaviour.h"
#include "GameState.h"
#include "Header/Math/Vector3.h"
#include "FlagController.h"
#include "PlayerBullet.h"

enum class BossState
{
	NONE,
	JOIN,
	HEALING,
	LEFT,
	JOINED,
	DEAD,
};

enum class BossMoveState
{
	NONE,
	// ê¸å`à⁄ìÆ
	LERP,
	// âÒì]à⁄ìÆ
	ROTATE,
};

enum class BossAttackState
{
	NONE,
	// çUåÇèÄîı
	PRE,
	// çUåÇ
	ATTACK,
	// çUåÇäJén
	END,
};

enum class BossAttackMode
{
	NONE,
	// ìÀêiçUåÇ
	CHARGE_ATTACK,
	// ÉoÉåÉbÉgçUåÇ
	BULLET_ATTACK,
};

class BossBehaviour : public GatesEngine::Behaviour
{
private:
	struct BulletInfo
	{
		Bullet* bullet;
		int index;
	};
private:
	GatesEngine::GameObject* target;

	BossState state;
	BossMoveState moveState;
	BossAttackState preAttackState;
	BossAttackState attackState;
	BossAttackMode attackMode;
private:
	GatesEngine::Math::Vector3 preLerpPos;

	FlagController decreaseHPFlagController;
	FlagController joinFlagController;
	FlagController leftFlagController;
	FlagController stopFlagController;
	FlagController chargeFlagController;
	FlagController chargeAttackFlagController;

	const float MAX_HP;
	float hp;
	float oldHP;

	bool isDead;
	float scale;
	float initScale;

	GatesEngine::Math::Vector3 chargeAttackVector;
	GatesEngine::Math::Vector3 startChargeAttackPos;
	int chargeAttackDrawCount;
	int chargeAttackFrameCount;

	FlagController nextAttackIntervalContrller;

	int bulletAttackStateShotCount;
	float shotInterval;

	int nextBulletIndex;
	float rotateBulletsValue;
	std::vector<BulletInfo> bullets;
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
	void Shot();
	void FixBulletPos();
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
	void AddBullet(Bullet* addBullet);
};

