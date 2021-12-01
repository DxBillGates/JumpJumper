#include "PlayerBullet.h"
#include "Header/Util/Random.h"

void PlayerBullet::Initialize()
{
	isUse = false;
	isHoming = false;
	randomVector = {};
	shotVector = {};
	vel = {};
	lifeTime = 0;
	homingTime = {};
}

void PlayerBullet::Update()
{
	const float MAX_LIFE_TIME = 5;
	if (isUse)
	{
		lifeTime += 0.016f;
		if (lifeTime >= MAX_LIFE_TIME)
		{
			Initialize();
		}
	}
}

PlayerBullet::PlayerBullet()
	: isUse(false)
	, vel({})
	, lifeTime(0)
	, isHoming(false)
	, randomVector({})
	, shotVector({})
	, homingTime(0)
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Shot(const GatesEngine::Math::Vector3& v)
{
	vel = v;
	shotVector = v;
	isUse = true;
}

void PlayerBullet::RandomVectorHomingShot(const GatesEngine::Math::Vector3& dirVector)
{
	float range = 32767;
	shotVector = GatesEngine::Math::Vector3::Normalize(dirVector);
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) ,GatesEngine::Random::Rand(-range,range) };
	randomVector = randomVector.Normalize();
	vel = randomVector;
	isUse = true;
	isHoming = true;
}

bool PlayerBullet::IsUse()
{
	return isUse;
}

void PlayerBullet::SetPos(const GatesEngine::Math::Vector3& p)
{
	setPos = p;
}

void PlayerBullet::SetTarget(const GatesEngine::Math::Vector3& t, const GatesEngine::Math::Axis& axis)
{
	float range = 32767;
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(10000,range) ,GatesEngine::Random::Rand(-range,-10000) };

	randomVector = randomVector.Normalize();

	GatesEngine::Math::Vector3 xVector = axis.x * randomVector.x;
	GatesEngine::Math::Vector3 yVector = axis.y * randomVector.y;
	GatesEngine::Math::Vector3 zVector = axis.z * randomVector.z;
	GatesEngine::Math::Vector3 vector = xVector + yVector + zVector;
	vel = vector.Normalize() * 2500;

	isUse = true;
	isHoming = true;
	target = t;

	float delayTime = GatesEngine::Random::Rand(0, 10);
	float delaySec = GatesEngine::Random::Rand(0, 10) / 100;
	homingTime = 1 + delayTime * delaySec / 100.0f;
}
