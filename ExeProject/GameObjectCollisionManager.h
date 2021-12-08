#pragma once
#include "Header/Component/Collider.h"
#include "Header/Graphics/Camera3D.h"
#include "PlayerBehaviour.h"

enum class GColliderType
{
	BLOCK,
	ENEMY,
	ENEMY_BULLET,
	PLAYER,
	PLAYER_BULLET,
	PLAYER_CAMERA,
	BOSS,
};

class GameObjectCollisionManager
{
private:
	std::vector<GatesEngine::Collider*> blockColliders;
	std::vector<GatesEngine::Collider*> enemyColliders;
	std::vector<GatesEngine::Collider*> playerColliders;
	std::vector<GatesEngine::Collider*> playerBulletColliders;
	std::vector<GatesEngine::Collider*> enemyBulletCollider;
	GatesEngine::Collider* bossCollider;

	GatesEngine::Camera3D* mainCamera;
	PlayerBehaviour* playerBehaviour;
	GatesEngine::Collider* mainCameraCollider;
private:
	void CheckCollisionHitBlockTo();
	void CheckCollisionHitEnemyTo();
	void CheckCollisionHitEnemyToCameraRay();
	void CheckCollisionHitBossTo();
	void CheckCollisionHitEnemyBulletTo();
public:
	void Update();
	GatesEngine::Collider* AddCollider(GatesEngine::Collider* collider, GColliderType type);
	void SetCamera(GatesEngine::Camera3D* mainCamera);
	void SetPlayerBehaviour(PlayerBehaviour* behaviour);
};

