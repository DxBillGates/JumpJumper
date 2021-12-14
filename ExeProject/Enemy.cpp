#include "Enemy.h"
#include "Header/Math/Easing.h"

Enemy::Enemy()
	: pos(GatesEngine::Math::Vector3())
	, vel(GatesEngine::Math::Vector3())
	, acc(GatesEngine::Math::Vector3())
	, target(GatesEngine::Math::Vector3())
	, setTime(0)
	, isUse(false)
	, isSetting(false)
	, isTarget(false)
{
}

Enemy::~Enemy()
{
}

void Enemy::Initialize()
{
	iPos = GatesEngine::Math::Vector3();
	pos = GatesEngine::Math::Vector3();
	vel = GatesEngine::Math::Vector3();
	acc = GatesEngine::Math::Vector3();
	target = GatesEngine::Math::Vector3();
	setTime = 0;
	isUse = false;
	isSetting = false;
	isTarget = false;
	isSpawning = true;
	spawnTime = 0;
}

void Enemy::Update()
{
	const float PER_FRAME = 1.0f / 60.0f;

	// スポーン処理
	if (spawnTime > 1)
	{
		isSpawning = false;
		spawnTime = 1;
	}
	if (isSpawning)spawnTime += PER_FRAME;


	// 指定されたポジションへ線形補間で移動
	if (isTarget)
	{
		float a = GatesEngine::Math::Easing::EaseOutExpo(1 - setTime);
		pos = GatesEngine::Math::Vector3::Lerp(iPos, target, a);
		if (setTime < 0)
		{
			isTarget = false;
			setTime = 0;
			isSetting = false;
			return;
		}

		setTime -= 0.016f / 2.0f;
	}
}

GatesEngine::Math::Vector3 Enemy::GetPosition()
{
	return pos;
}

bool Enemy::GetIsUse()
{
	return isUse;
}

bool Enemy::GetIsSetting()
{
	return isSetting;
}

bool Enemy::GetIsTarget()
{
	return isTarget;
}

void Enemy::SetPosition(const GatesEngine::Math::Vector3& pos)
{
	this->pos = pos;
}

void Enemy::SetVelocity(const GatesEngine::Math::Vector3& vel)
{
	this->vel = vel;
}

void Enemy::SetTime(float value)
{
	setTime = value;
}

void Enemy::SetIsUse(bool flag)
{
	isUse = flag;
}

void Enemy::SetTarget(const GatesEngine::Math::Vector3& targetPos, const GatesEngine::Math::Vector3& addVector)
{
	target = targetPos;
	vel = addVector;
	isTarget = true;
	isSetting = true;
}
