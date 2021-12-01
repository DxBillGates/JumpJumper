#include "Enemy.h"

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
	pos = GatesEngine::Math::Vector3();
	vel = GatesEngine::Math::Vector3();
	acc = GatesEngine::Math::Vector3();
	target = GatesEngine::Math::Vector3();
	setTime = 0;
	isUse = false;
	isSetting = false;
	isTarget = false;
}

void Enemy::Update()
{
	//pos += vel;
	if (isTarget)
	{
		if (setTime < 0)
		{
			isTarget = false;
			isSetting = false;
			return;
		}
		GatesEngine::Math::Vector3 diff = target - pos;
		GatesEngine::Math::Vector3 acc;
		acc += (diff - vel * setTime) * 2.0f / (setTime * setTime);

		setTime -= 0.016f / 2.0f;

		vel += acc * 0.016f / 2.0f;
		pos += vel * 0.016f / 2.0f;
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
