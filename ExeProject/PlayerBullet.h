#pragma once
#include "Header/Math/Math.h"

class PlayerBullet
{
protected:
	bool isUse;
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Math::Vector3 setPos;
	float lifeTime;
protected:
	void Initialize();
	void Update();
public:
	PlayerBullet();
	virtual ~PlayerBullet();
	void Shot(const GatesEngine::Math::Vector3& v);
	bool IsUse();
	void SetPos(const GatesEngine::Math::Vector3& p);
};