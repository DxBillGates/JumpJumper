#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Graphics/Camera.h"
#include "Enemy.h"
#include "PlayerBullet.h"

class NormalEnemyBehaviour : public GatesEngine::Behaviour, public Enemy
{
private:
	GatesEngine::GameObject* target;
	GatesEngine::GameObject* boss;
	float hp;
	float t;

	bool isTargetInCollder;

	std::vector<Bullet*> bullets;
	float shotInterval;
private:
	void Shot();
	void FixBulletPos();
public:
	NormalEnemyBehaviour();
	~NormalEnemyBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::Collider* otherCollider) override;
	void Damage(float value = 1);
	float GetHP();
	void SetBoss(GatesEngine::GameObject* gameObject);
	void SetTarget(GatesEngine::GameObject* t);
	void AddBullet(Bullet* addBullet);
};