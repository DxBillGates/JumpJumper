#include "BossBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"
#include "PlayerCamera.h"

void BossBehaviour::PreAttack()
{
	switch (attackMode)
	{
	case BossAttackMode::NONE:
		break;
	case BossAttackMode::CHARGE_ATTACK:
		PreChargeAttack();
		break;
	default:
		break;
	}
}

void BossBehaviour::Attack()
{
	switch (attackMode)
	{
	case BossAttackMode::NONE:
		break;
	case BossAttackMode::CHARGE_ATTACK:
		ChargeAttack();
		break;
	case BossAttackMode::BULLET_ATTACK:
		Shot();
		break;
	default:
		break;
	}
}

void BossBehaviour::EndAttack()
{
	attackState = BossAttackState::NONE;
	attackMode = BossAttackMode::NONE;
	nextAttackIntervalContrller.Initialize();
	nextAttackIntervalContrller.SetFlag(true);
}

void BossBehaviour::SetState()
{
	nextAttackIntervalContrller.SetMaxTimeProperty(1);
	// 攻撃ステート変更
	if (attackState == BossAttackState::NONE && preAttackState == BossAttackState::NONE)
	{
		// 次の攻撃インターバルが既定値を超えた際に攻撃に移る
		if (nextAttackIntervalContrller.GetOverTimeTrigger())
		{
			const float CHARGE_ATTACK_DISTAMCE = 3000;
			float targetDistance = CalcTargetDistance();

			// 各種攻撃パターン
			if (targetDistance >= CHARGE_ATTACK_DISTAMCE)
			{
				SetAttackMode(BossAttackMode::CHARGE_ATTACK);
				chargeFlagController.SetFlag(true);
			}
			else
			{
				SetAttackMode(BossAttackMode::BULLET_ATTACK);
				attackState = BossAttackState::ATTACK;
			}
		}
	}

	// 移動ステート変更
	if (moveState == BossMoveState::NONE)
	{

	}
}

void BossBehaviour::SetAttackMode(BossAttackMode setAttackMode)
{
	attackMode = setAttackMode;
	attackState = BossAttackState::PRE;
}

void BossBehaviour::SetMoveState(BossMoveState setMoveState)
{
	moveState = setMoveState;
}

float BossBehaviour::CalcTargetDistance()
{
	if (!target)return 0;

	float length = GatesEngine::Math::Vector3::Distance(target->GetTransform()->position, gameObject->GetTransform()->position);
	return length;
}

void BossBehaviour::InitState()
{
	state = BossState::NONE;
	joinFlagController.Initialize();
	joinFlagController.SetTime(1);
	leftFlagController.Initialize();
}

void BossBehaviour::JoinOrLeft(BossState state)
{
	GatesEngine::Math::Vector3 endPos;
	float easingValue = 0;

	// PER_FRAME / MAX_TIMEを加算することでMAX_TIMEに指定した時間までの1フレームの経過時間を取得
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_TIME = 3;
	joinFlagController.SetMaxTimeProperty(MAX_TIME);
	leftFlagController.SetMaxTimeProperty(MAX_TIME);

	// ボス戦に移行したとき、ザコ敵戦に移行したときの処理、線形補間で移動
	switch (state)
	{
	case BossState::JOIN:
		endPos = {};
		easingValue = GatesEngine::Math::Easing::EaseInExpo(joinFlagController.GetTime());
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		break;
	case BossState::LEFT:
		endPos = preLerpPos;
		endPos.y = preLerpPos.y + 10000;
		easingValue = GatesEngine::Math::Easing::EaseOutExpo(leftFlagController.GetTime());
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		break;
	default:
		break;
	}

	// 設定した時間を経過したらステートをもとに戻す
	if (joinFlagController.IsOverTime() || leftFlagController.IsOverTime())
	{
		InitState();
		if (state == BossState::JOIN)
		{
			stopFlagController.SetFlag(true);
			this->state = BossState::JOINED;
		}
	}
}

void BossBehaviour::Stoping()
{
	// HPを回復させる処理
	state = BossState::HEALING;
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_STOPING_TIME = 5;

	const int HEALING_VALUE_PER_FRAME = 1;

	hp = GatesEngine::Math::Lerp(0, MAX_HP, GatesEngine::Math::Easing::EaseInOutQuint(stopFlagController.GetTime()));
	oldHP = hp;
	if (stopFlagController.GetTime() >= 1)
	{
		state = BossState::JOINED;
		stopFlagController.Initialize();
		hp = MAX_HP;
		nextAttackIntervalContrller.SetFlag(true);
	}

	stopFlagController.SetMaxTimeProperty(MAX_STOPING_TIME);
}

void BossBehaviour::PreChargeAttack()
{
	if (!target) return;
	if (!chargeFlagController.GetFlag())return;

	const float MAX_CHARGE_TIME = 3;
	const float PER_FRAME = 1.0f / 60.0f;
	chargeFlagController.SetMaxTimeProperty(MAX_CHARGE_TIME);

	chargeAttackVector = target->GetTransform()->position - gameObject->GetTransform()->position;
	startChargeAttackPos = gameObject->GetTransform()->position;
	if (chargeFlagController.IsOverTime())
	{
		chargeFlagController.Initialize();
		chargeAttackFlagController.SetFlag(true);
		chargeAttackVector = target->GetTransform()->position - gameObject->GetTransform()->position;
		//chargeAttackVector = chargeAttackVector.Normalize();
		startChargeAttackPos = gameObject->GetTransform()->position;
		attackState = BossAttackState::ATTACK;
	}

	float x = GatesEngine::Random::Rand(-100, 100);
	float y = GatesEngine::Random::Rand(-100, 100);
	float z = GatesEngine::Random::Rand(-100, 100);
	gameObject->GetTransform()->position += GatesEngine::Math::Vector3(x, y, z).Normalize() * 10;



	int incrementChargeFrameCount = 3;

	if (chargeAttackFrameCount % incrementChargeFrameCount == 0)
	{
		++chargeAttackDrawCount;
	}
	++chargeAttackFrameCount;


}

void BossBehaviour::ChargeAttack()
{
	if (!chargeAttackFlagController.GetFlag())return;

	const float MAX_ATTACK_TIME = 1;
	const float PER_FRAME = 1.0f / 60.0f;
	chargeAttackFlagController.SetMaxTimeProperty(MAX_ATTACK_TIME);

	if (chargeAttackFlagController.IsOverTime())
	{
		//chargeFlagController.SetFlag(true);
		chargeAttackFlagController.Initialize();
		chargeAttackVector = GatesEngine::Math::Vector3();
		chargeAttackDrawCount = 0;
		chargeAttackFrameCount = 0;
		EndAttack();
		return;
	}

	const float ATTACK_VALUE = 1;
	GatesEngine::Math::Vector3 endLerpPos = startChargeAttackPos + chargeAttackVector * ATTACK_VALUE;
	gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(startChargeAttackPos, endLerpPos, GatesEngine::Math::Easing::EaseInBack(chargeAttackFlagController.GetTime()));

	int incrementChargeFrameCount = 3;

	if (chargeAttackFrameCount % incrementChargeFrameCount == 0)
	{
		--chargeAttackDrawCount;
		if (chargeAttackDrawCount < 0)
		{
			chargeAttackDrawCount = 0;
		}
	}
	++chargeAttackFrameCount;
}

void BossBehaviour::Shot()
{
	const float INTERVAL = 0.2f;
	if (shotInterval > INTERVAL)
	{
		shotInterval = 0;
	}
	else
	{
		shotInterval += 0.016f / 2.0f;
		return;
	}


	bool isShot = false;
	for (auto& b : bullets)
	{
		if (b.bullet->IsUse())continue;

		if (!isShot)
		{
			if (!target)continue;
			b.bullet->SetTarget(target, 0.1f, GatesEngine::Math::Vector3(0, 1, 0), 5000);
			isShot = true;
			++bulletAttackStateShotCount;
		}
		else
		{
			b.bullet->SetPos(gameObject->GetTransform()->position);
		}
	}

	const int MAX_SHOT_COUNT = 5;
	if (bulletAttackStateShotCount >= MAX_SHOT_COUNT)
	{
		EndAttack();
		bulletAttackStateShotCount = 0;
	}
}

void BossBehaviour::FixBulletPos()
{
	int bulltesSize = (int)bullets.size();
	// 使っていない弾の位置を補正
	for (auto& b : bullets)
	{
		if (b.bullet->IsUse())continue;

		// 登録されているインデックス情報からボスの周囲に配置
		GatesEngine::Math::Vector3 setPos;
		const float RADIUS = 1000;
		setPos.x = RADIUS * sinf((2 * GatesEngine::Math::PI / bulltesSize) * (b.index + rotateBulletsValue)) / 2;
		setPos.y = 0;
		setPos.z = RADIUS * cosf((2 * GatesEngine::Math::PI / bulltesSize) * (b.index + rotateBulletsValue)) / 2;
		b.bullet->SetPos(gameObject->GetTransform()->position + setPos);
	}

	const float PER_FRAME = 1.0f / 60.0f;
	const float ROTATE_SPEED = 5;
	rotateBulletsValue += PER_FRAME * ROTATE_SPEED;
}

BossBehaviour::BossBehaviour()
	: state(BossState::NONE)
	, MAX_HP(100)
	, hp(MAX_HP)
{
}

BossBehaviour::~BossBehaviour()
{
}

void BossBehaviour::Start()
{
	InitState();

	attackState = BossAttackState::NONE;
	attackMode = BossAttackMode::NONE;

	stopFlagController.Initialize();
	hp = 0;
	oldHP = hp;
	gameObject->GetTransform()->position = { 0,10000,0 };
	gameObject->GetTransform()->scale = 500;
	scale = gameObject->GetTransform()->scale.x;
	isDead = false;

	decreaseHPFlagController.Initialize();
	chargeFlagController.Initialize();
	chargeAttackFlagController.Initialize();
	chargeAttackVector = {};
	nextAttackIntervalContrller.Initialize();
}

void BossBehaviour::Update()
{
	preAttackState = attackState;

	if (joinFlagController.GetFlag())JoinOrLeft(BossState::JOIN);
	else if (leftFlagController.GetFlag())JoinOrLeft(BossState::LEFT);

	// ボスのステートがJOINからNONEに変わりstopFlag(回復モード)になったフレームに画面揺れを発生させる(着地時)
	if (state == BossState::JOINED && stopFlagController.GetFlag())
	{
		PlayerCamera* mainCamera = dynamic_cast<PlayerCamera*>(gameObject->GetGraphicsDevice()->GetMainCamera());

		if (mainCamera)
		{
			mainCamera->SetShake(0.5f / 2, 100);
		}
	}

	// 登場した時の回復演出
	if (stopFlagController.GetFlag())Stoping();
	// ライフ回復
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_DECREASE_HP_VALUE = 1;
	decreaseHPFlagController.SetMaxTimeProperty(MAX_DECREASE_HP_VALUE);

	if (decreaseHPFlagController.IsOverTime())decreaseHPFlagController.SetFlag(false);;


	// 移動、攻撃処理
	if (!isDead)
	{
		// 攻撃処理
		if (attackState == BossAttackState::PRE)PreAttack();
		else if (attackState == BossAttackState::ATTACK)Attack();

		if (state == BossState::JOINED && state != BossState::DEAD)
		{
			SetState();
		}
	}


	// スケール変化終了
	if (scale <= 0)
	{
		scale = 0;
		isDead = false;
	}


	// 死んだときのスケール変化
	gameObject->GetTransform()->scale = scale;
	if (isDead)
	{
		scale -= 1;
	}

	// 各種フラグコントローラーの更新
	decreaseHPFlagController.Update(PER_FRAME);
	joinFlagController.Update(PER_FRAME);
	leftFlagController.Update(PER_FRAME);
	stopFlagController.Update(PER_FRAME);

	if (state == BossState::JOINED && state != BossState::DEAD)
	{
		chargeFlagController.Update(PER_FRAME);
		chargeAttackFlagController.Update(PER_FRAME);
		nextAttackIntervalContrller.Update(PER_FRAME);
	}

	FixBulletPos();
}

void BossBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0.5f,0,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();
}

void BossBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	float persent = hp / MAX_HP - 0.05f;
	float oldPersent = oldHP / MAX_HP - 0.05f;
	float maxPersent = 1 - 0.05f;

	float easeInExpo = GatesEngine::Math::Easing::EaseInExpo(decreaseHPFlagController.GetTime());
	float easeOutElastic = GatesEngine::Math::Easing::EaseOutElastic(decreaseHPFlagController.GetTime());
	float resultPersent = GatesEngine::Math::Lerp(oldPersent, persent, easeOutElastic);

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920 * resultPersent,50,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 - 50,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920 * maxPersent,50,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 - 50,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(0, 0, 0, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();

	if (attackMode == BossAttackMode::CHARGE_ATTACK && !isDead)
	{
		const int MAX_DRAW_CHARGE_OBJECT = 30;
		float objectDistance = chargeAttackVector.Length() / MAX_DRAW_CHARGE_OBJECT;
		GatesEngine::Math::Vector3 vec;
		GatesEngine::Math::Vector3 startPos;
		if (attackState == BossAttackState::PRE)
		{
			vec = chargeAttackVector;
			startPos = startChargeAttackPos;
		}
		else if (attackState == BossAttackState::ATTACK)
		{
			vec = -chargeAttackVector;
			startPos = startChargeAttackPos + chargeAttackVector.Normalize() * objectDistance * 30;
		}
		chargeAttackDrawCount = chargeAttackDrawCount >= MAX_DRAW_CHARGE_OBJECT ? MAX_DRAW_CHARGE_OBJECT : chargeAttackDrawCount;

		for (int i = 0; i < chargeAttackDrawCount; ++i)
		{
			GatesEngine::Math::Vector3 pos = startPos + vec.Normalize() * objectDistance * i;
			GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultMeshShader")->Set();
			graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 10,10,10 }) * GatesEngine::Math::Matrix4x4::Translate(pos));
			graphicsDevice->GetMainCamera()->Set(2);
			graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{0,0.5f,0,1} });
			GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();
		}
	}

}

void BossBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	GatesEngine::GameObject* object = otherCollider->GetGameObject();

	if (object->GetTag() == "playerBullet")
	{
		oldHP = hp;
		hp -= MAX_HP / 10;
		decreaseHPFlagController.Initialize();
		decreaseHPFlagController.SetFlag(true);
		if (hp <= 0)
		{
			isDead = true;
			state = BossState::DEAD;
		}
	}
}

void BossBehaviour::SetBossState(BossState state)
{
	InitState();
	this->state = state;
	preLerpPos = gameObject->GetTransform()->position;

	switch (state)
	{
	case BossState::NONE:
		joinFlagController.Initialize();
		leftFlagController.Initialize();
		break;
	case BossState::JOIN:
		joinFlagController.SetFlag(true);
		joinFlagController.SetTime(0);
		break;
	case BossState::LEFT:
		leftFlagController.SetFlag(true);
		leftFlagController.SetTime(0);
		break;
	}
}

BossState BossBehaviour::GetState()
{
	return state;
}

float BossBehaviour::GetHp()
{
	return hp;
}

bool BossBehaviour::GetIsEndScaleAnimation()
{
	bool flag = false;

	if (scale <= 0)
	{
		flag = true;
	}
	return flag;
}

void BossBehaviour::SetInitScale(float value)
{
	initScale = value;
}

bool BossBehaviour::GetIsDead()
{
	return isDead;
}

void BossBehaviour::SetTarget(GatesEngine::GameObject* targetObject)
{
	target = targetObject;
}

void BossBehaviour::AddBullet(Bullet* addBullet)
{
	bullets.push_back({ addBullet, nextBulletIndex });
	++nextBulletIndex;
}
