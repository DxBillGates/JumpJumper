#include "PlayerBullet.h"
#include "Header/Util/Random.h"

void Bullet::Initialize()
{
	setPos = { 0,-1000,0 };
	isUse = false;
	isHoming = false;
	randomVector = {};
	shotVector = {};
	vel = {};
	lifeTime = 0;
	homingTime = {};
	maxHomingTime = 0;
}

void Bullet::Update()
{
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_LIFE_TIME = 5;
	if (isUse)
	{
		lifeTime += 0.016f;
		if (lifeTime >= MAX_LIFE_TIME)
		{
			Initialize();
		}

		if (!isEmittion)return;
		if (emittionTime >= setEmittionTime)
		{
			isEmittion = false;
			emittionTime = 0;
			setEmittionTime = 0;

			float range = 32767;
			randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) ,GatesEngine::Random::Rand(-range,-1) };


			GatesEngine::Math::Vector3 xVector = moveAxis.x * randomVector.x;
			GatesEngine::Math::Vector3 yVector = moveAxis.y * randomVector.y;
			GatesEngine::Math::Vector3 zVector = moveAxis.z * randomVector.z;
			GatesEngine::Math::Vector3 vector = xVector * 1000 + yVector * 1000 + zVector;
			vel = vector.Normalize() * 10;
		}


		emittionTime += PER_FRAME;
	}
}

Bullet::Bullet()
	: isUse(false)
	, vel({})
	, lifeTime(0)
	, isHoming(false)
	, randomVector({})
	, shotVector({})
	, homingTime(0)
	, setPos({ 0,-1000,0 })
{
}

Bullet::~Bullet()
{
}

void Bullet::Shot(const GatesEngine::Math::Vector3& v)
{
	vel = v;
	shotVector = v;
	isUse = true;
}

void Bullet::Shot(const GatesEngine::Math::Vector3& v, float emittionTime, const GatesEngine::Math::Axis& axis)
{
	vel = v;
	shotVector = v;
	isUse = true;
	isEmittion = true;
	this->emittionTime = 0;
	setEmittionTime = emittionTime;
	moveAxis = axis;
}

void Bullet::RandomVectorHomingShot(const GatesEngine::Math::Vector3& dirVector)
{
	float range = 32767;
	shotVector = GatesEngine::Math::Vector3::Normalize(dirVector);
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) ,GatesEngine::Random::Rand(-range,range) };
	randomVector = randomVector.Normalize();
	vel = randomVector;
	isUse = true;
	isHoming = true;
}

bool Bullet::IsUse()
{
	return isUse;
}

void Bullet::SetPos(const GatesEngine::Math::Vector3& p)
{
	setPos = p;
}

void Bullet::SetTarget(GatesEngine::GameObject* t, float maxTime, const GatesEngine::Math::Axis& axis, const GatesEngine::Math::Vector3& addValue)
{
	float range = 32767;
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(10000,range) ,GatesEngine::Random::Rand(-range,-10000) };

	randomVector = randomVector.Normalize();

	GatesEngine::Math::Vector3 xVector = axis.x * randomVector.x;
	GatesEngine::Math::Vector3 yVector = axis.y * randomVector.y;
	GatesEngine::Math::Vector3 zVector = axis.z * randomVector.z;
	GatesEngine::Math::Vector3 vector = xVector + yVector + zVector;
	vel = vector.Normalize();
	vel.x *= addValue.x;
	vel.y *= addValue.y;
	vel.z *= addValue.z;

	isUse = true;
	isHoming = true;
	target = t;
	maxHomingTime = maxTime;
	float delayTime = GatesEngine::Random::Rand(0, 10);
	float delaySec = GatesEngine::Random::Rand(0, 10) / 100;
	homingTime = 1 + delayTime * delaySec / 100.0f;
}

void Bullet::SetTarget(GatesEngine::GameObject* t, float maxTime, const GatesEngine::Math::Vector3& vec, float addValue)
{
	float range = 32767;
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(10000,range) ,GatesEngine::Random::Rand(-range,-10000) };

	randomVector = randomVector.Normalize();

	GatesEngine::Math::Vector3 vector = vec * randomVector.y;
	vel = vector.Normalize() * addValue;

	isUse = true;
	isHoming = true;
	target = t;
	maxHomingTime = maxTime;
	float delayTime = GatesEngine::Random::Rand(0, 10);
	float delaySec = GatesEngine::Random::Rand(0, 10) / 100;
	homingTime = 1 + delayTime * delaySec / 100.0f;
}
