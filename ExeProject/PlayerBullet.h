#pragma once
#include "Header/Math/Math.h"

class PlayerBullet
{
protected:
	bool isUse;
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Math::Vector3 setPos;
	float lifeTime;
	bool isHoming;
	GatesEngine::Math::Vector3 randomVector;
	GatesEngine::Math::Vector3 shotVector;

	GatesEngine::Math::Vector3 target;
	float homingTime;
protected:
	void Initialize();
	void Update();
public:
	PlayerBullet();
	virtual ~PlayerBullet();
	void Shot(const GatesEngine::Math::Vector3& v);
	void RandomVectorHomingShot(const GatesEngine::Math::Vector3& dirVector);
	bool IsUse();
	void SetPos(const GatesEngine::Math::Vector3& p);
	void SetTarget(const GatesEngine::Math::Vector3& t, const GatesEngine::Math::Axis& axis = {});
};