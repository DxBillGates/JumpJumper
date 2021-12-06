#pragma once
#include "Header/GameObject/GameObject.h"
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

	GatesEngine::GameObject* target;
	float homingTime;
	float maxHomingTime;
public:
	void Initialize();
	void Update();
public:
	PlayerBullet();
	virtual ~PlayerBullet();
	void Shot(const GatesEngine::Math::Vector3& v);
	void RandomVectorHomingShot(const GatesEngine::Math::Vector3& dirVector);
	bool IsUse();
	void SetPos(const GatesEngine::Math::Vector3& p);
	void SetTarget(GatesEngine::GameObject* t, float maxTime = 0 , const GatesEngine::Math::Axis& axis = {});
};