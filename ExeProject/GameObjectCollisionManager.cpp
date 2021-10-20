#include "GameObjectCollisionManager.h"
#include "Header/Collision/CollisionManager.h"
#include "Header/GameObject/GameObject.h"

void GameObjectCollisionManager::CheckCollisionHitBlockTo()
{
	//ブロックとプレイヤー、敵、玉の当たり判定
	for (auto b : blockColliders)
	{
		for (auto p : playerColliders)
		{
			GatesEngine::GameObject* block, * player;
			block = b->GetGameObject();
			player = p->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(block->GetCollider(), player->GetCollider()))
			{
				block->Collision(player);
				block->GetCollider()->SetColor({ 1,0,0,0 });
				player->Collision(block);
				player->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}

		for (auto p : enemyColliders)
		{
			GatesEngine::GameObject* block, * enemy;
			block = b->GetGameObject();
			enemy = p->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(block->GetCollider(), enemy->GetCollider()))
			{
				block->Collision(enemy);
				block->GetCollider()->SetColor({ 1,0,0,0 });
				enemy->Collision(block);
				enemy->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}

		for (auto pb : playerBulletColliders)
		{
			GatesEngine::GameObject* block, * playerBullet;
			block = b->GetGameObject();
			playerBullet = pb->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(block->GetCollider(), playerBullet->GetCollider()))
			{
				block->Collision(playerBullet);
				block->GetCollider()->SetColor({ 1,0,0,0 });
				playerBullet->Collision(block);
				playerBullet->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}
	}
}

void GameObjectCollisionManager::CheckCollisionHitEnemyTo()
{
	//敵とプレイヤー、玉
	for (auto e : enemyColliders)
	{
		for (auto p : playerColliders)
		{
			GatesEngine::GameObject* enemy, * player;
			enemy = e->GetGameObject();
			player = p->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(enemy->GetCollider(), player->GetCollider()))
			{
				enemy->Collision(player);
				enemy->GetCollider()->SetColor({1,0,0,0});
				player->Collision(enemy);
				player->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}

		for (auto pb : playerBulletColliders)
		{
			GatesEngine::GameObject* enemy, * playerBullet;
			enemy = e->GetGameObject();
			playerBullet = pb->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(enemy->GetCollider(), playerBullet->GetCollider()))
			{
				enemy->Collision(playerBullet);
				enemy->GetCollider()->SetColor({ 1,0,0,0 });
				playerBullet->Collision(enemy);
				playerBullet->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}
	}
}

void GameObjectCollisionManager::Update()
{
	CheckCollisionHitBlockTo();
	CheckCollisionHitEnemyTo();
}

void GameObjectCollisionManager::AddCollider(GatesEngine::Collider* collider, GColliderType type)
{
	switch (type)
	{
	case GColliderType::BLOCK:
		blockColliders.push_back(collider);
		break;
	case GColliderType::ENEMY:
		enemyColliders.push_back(collider);
		break;
	case GColliderType::PLAYER:
		playerColliders.push_back(collider);
		break;
	case GColliderType::PLAYER_BULLET:
		playerBulletColliders.push_back(collider);
		break;
	default:
		break;
	}
}
