#pragma once
#include "Header/Component/Collider.h"

enum class GColliderType
{
	BLOCK,
	ENEMY,
	PLAYER,
	PLAYER_BULLET
};

class GameObjectCollisionManager
{
private:
	std::vector<GatesEngine::Collider*> blockColliders;
	std::vector<GatesEngine::Collider*> enemyColliders;
	std::vector<GatesEngine::Collider*> playerColliders;
	std::vector<GatesEngine::Collider*> playerBulletColliders;
private:
	void CheckCollisionHitBlockTo();
	void CheckCollisionHitEnemyTo();
public:
	void Update();
	void AddCollider(GatesEngine::Collider* collider,GColliderType type);
};

