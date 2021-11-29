#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/Camera3D.h"

class BossBehaviour : public GatesEngine::Behaviour
{
private:
	const int MAX_HP;
	int hp;
	GatesEngine::Camera3D* mainCamera;
	bool callEnemyFlag;
	const int MAX_NORMAL_ENEMY;
	int normalEnemyAddedCount;
	std::vector<GatesEngine::GameObject*> normalEnemies;
private:
	void CallEnemy(const GatesEngine::Math::Vector3& centerPos, int count = 5);
public:
	BossBehaviour();
	~BossBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::Collider* otherCollider);
	void AddNormalEnemy(GatesEngine::GameObject* enemy);
};

