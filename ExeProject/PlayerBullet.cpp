#include "PlayerBullet.h"

void PlayerBullet::Initialize()
{
	isUse = false;
	vel = {};
	lifeTime = 0;
}

void PlayerBullet::Update()
{
	const float MAX_LIFE_TIME = 10;
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
{
}

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Shot(const GatesEngine::Math::Vector3& v)
{
	vel = v;
	isUse = true;
}

bool PlayerBullet::IsUse()
{
	return isUse;
}

void PlayerBullet::SetPos(const GatesEngine::Math::Vector3& p)
{
	setPos = p;
}
