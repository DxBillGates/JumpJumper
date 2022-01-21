#include "BossBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"
#include "PlayerCamera.h"

void BossBehaviour::InitState()
{
	state = BossState::NONE;
	isJoining = false;
	joiningTime = 1;
	isLefting = false;
	leftingTime = 1;
}

void BossBehaviour::JoinOrLeft(BossState state)
{
	GatesEngine::Math::Vector3 endPos;
	float easingValue = 0;

	// PER_FRAME / MAX_TIMEを加算することでMAX_TIMEに指定した時間までの1フレームの経過時間を取得
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_TIME = 3;


	// ボス戦に移行したとき、ザコ敵戦に移行したときの処理、線形補間で移動
	switch (state)
	{
	case BossState::JOIN:
		endPos = {};
		easingValue = GatesEngine::Math::Easing::EaseInExpo(joiningTime);
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		joiningTime += PER_FRAME / MAX_TIME;
		break;
	case BossState::LEFT:
		endPos = preLerpPos;
		endPos.y = preLerpPos.y + 10000;
		easingValue = GatesEngine::Math::Easing::EaseOutExpo(leftingTime);
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		leftingTime += PER_FRAME / MAX_TIME;
		break;
	default:
		break;
	}

	// 設定した時間を経過したらステートをもとに戻す
	if (joiningTime > 1 || leftingTime > 1)
	{
		if (state == BossState::JOIN)
		{
			stopFlag = true;
		}
		InitState();
	}


}

void BossBehaviour::Stoping()
{
	// HPを回復させる処理
	state = BossState::HEALING;
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_STOPING_TIME = 5;

	const int HEALING_VALUE_PER_FRAME = 1;

	hp = GatesEngine::Math::Lerp(0, MAX_HP, GatesEngine::Math::Easing::EaseInOutQuint(stopingTime));
	oldHP = hp;
	if (stopingTime >= 1)
	{
		state = BossState::NONE;
		stopFlag = false;
		stopingTime = 0;
		hp = MAX_HP;
		chargeFlag = true;
	}

	stopingTime += PER_FRAME / MAX_STOPING_TIME;
}

void BossBehaviour::PreChargeAttack()
{
	if (!target) return;
	if (!chargeFlag)return;

	const float MAX_CHARGE_TIME = 3;
	const float PER_FRAME = 1.0f / 60.0f;


	// 攻撃フラグをセット
	if (chargeTime >= 1)
	{
		chargeFlag = false;
		chargeTime = 0;
		chargeAttackFlag = true;
		chargeAttackVector = target->GetTransform()->position - gameObject->GetTransform()->position;
		//chargeAttackVector = chargeAttackVector.Normalize();
		startChargeAttackPos = gameObject->GetTransform()->position;
	}

	float x = GatesEngine::Random::Rand(-100, 100);
	float y = GatesEngine::Random::Rand(-100, 100);
	float z = GatesEngine::Random::Rand(-100, 100);
	gameObject->GetTransform()->position += GatesEngine::Math::Vector3(x, y, z).Normalize() * 10;
	chargeTime += PER_FRAME / MAX_CHARGE_TIME;


}

void BossBehaviour::ChargeAttack()
{
	if (!chargeAttackFlag)return;

	const float MAX_ATTACK_TIME = 1;
	const float PER_FRAME = 1.0f / 60.0f;

	if (chargeAttackTime > 1)
	{
		chargeFlag = true;
		chargeAttackFlag = false;
		chargeAttackTime = 0;
		chargeAttackVector = GatesEngine::Math::Vector3();
		return;
	}

	const float ATTACK_VALUE = 1;
	GatesEngine::Math::Vector3 endLerpPos = startChargeAttackPos + chargeAttackVector * ATTACK_VALUE;
	gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(startChargeAttackPos,endLerpPos, GatesEngine::Math::Easing::EaseInBack(chargeAttackTime));

	chargeAttackTime += PER_FRAME / MAX_ATTACK_TIME;
}

BossBehaviour::BossBehaviour()
	: state(BossState::NONE)
	, isJoining(false)
	, joiningTime(0)
	, isLefting(false)
	, leftingTime(0)
	, stopFlag(false)
	, stopingTime(0)
	, MAX_HP(100)
	, hp(MAX_HP)
	, decreaseHpTime(0)
	, MAX_DECREASE_HP_TIME(1)
{
}

BossBehaviour::~BossBehaviour()
{
}

void BossBehaviour::Start()
{
	InitState();
	stopFlag = false;
	stopingTime = 0;
	hp = 0;
	oldHP = hp;
	gameObject->GetTransform()->position = { 0,10000,0 };
	gameObject->GetTransform()->scale = 500;
	scale = gameObject->GetTransform()->scale.x;
	isDead = false;

	chargeFlag = false;
	chargeAttackFlag = false;
	chargeAttackTime = 0;
	chargeTime = 0;
	chargeAttackVector = {};
}

void BossBehaviour::Update()
{
	if (isJoining)JoinOrLeft(BossState::JOIN);
	else if (isLefting)JoinOrLeft(BossState::LEFT);

	// ボスのステートがJOINからNONEに変わりstopFlag(回復モード)になったフレームに画面揺れを発生させる(着地時)
	if (state == BossState::NONE && stopFlag)
	{
		PlayerCamera* mainCamera = dynamic_cast<PlayerCamera*>(gameObject->GetGraphicsDevice()->GetMainCamera());

		if (mainCamera)
		{
			mainCamera->SetShake(0.5f / 2, 100);
		}
	}

	// 登場した時の回復演出
	if (stopFlag)Stoping();

	PreChargeAttack();
	ChargeAttack();

	// ライフ回復
	const float PER_FRAME = 1.0f / 60.0f;
	if (decreaseHpTime >= 1)decreaseHpTimeFlag = false;
	if (decreaseHpTimeFlag)decreaseHpTime += PER_FRAME / MAX_DECREASE_HP_TIME;


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

}

void BossBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0.5f,0,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();


	//GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(500);
	//GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(1000, 0, 0));

	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0.5f,0,1} });
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();


	//posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(-1000, 0, 0));

	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0.5f,0,1} });
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();
}

void BossBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	float persent = hp / MAX_HP - 0.05f;
	float oldPersent = oldHP / MAX_HP - 0.05f;
	float maxPersent = 1 - 0.05f;

	float easeInExpo = GatesEngine::Math::Easing::EaseInExpo(decreaseHpTime);
	float easeOutElastic = GatesEngine::Math::Easing::EaseOutElastic(decreaseHpTime);
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

}

void BossBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	GatesEngine::GameObject* object = otherCollider->GetGameObject();

	if (object->GetTag() == "playerBullet")
	{
		oldHP = hp;
		hp -= MAX_HP / 10;
		const float PER_FRAME = 1.0f / 60.0f;
		decreaseHpTimeFlag = true;
		decreaseHpTime = 0;
		if (hp <= 0)
		{
			isDead = true;
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
		isJoining = false;
		joiningTime = 0;
		isLefting = false;
		leftingTime = 0;
		break;
	case BossState::JOIN:
		isJoining = true;
		joiningTime = 0;
		break;
	case BossState::LEFT:
		isLefting = true;
		leftingTime = 0;
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
