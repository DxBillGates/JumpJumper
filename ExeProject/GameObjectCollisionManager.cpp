#include "GameObjectCollisionManager.h"
#include "Header/Collision/CollisionManager.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Input/Input.h"

void GameObjectCollisionManager::CheckCollisionHitBlockTo()
{
	//ブロックとプレイヤー、敵、玉の当たり判定
	for (auto& b : blockColliders)
	{
		if (!b->GetEnabled())continue;
		//複数コライダー対応
		for (auto& p : playerColliders)
		{
			if (!p->GetEnabled())continue;
			GatesEngine::GameObject* block, * player;
			block = b->GetGameObject();
			player = p->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(b, p))
			{
				block->Collision(p);
				b->SetColor({ 1,0,0,0 });
				player->Collision(b);
				p->SetColor({ 1,0,0,0 });
			}
		}

		for (auto& e : enemyColliders)
		{
			if (!e->GetEnabled())continue;
			GatesEngine::GameObject* block, * enemy;
			block = b->GetGameObject();
			enemy = e->GetGameObject();
			if (e->GetType() == GatesEngine::ColliderType::CUBE)
			{
				if (GatesEngine::CollisionManager::CheckAABB(b, e))
				{
					block->Collision(e);
					b->SetColor({ 1,0,0,0 });
					enemy->Collision(b);
					e->SetColor({ 1,0,0,0 });
				}
			}
		}

		for (auto& pb : playerBulletColliders)
		{
			if (!pb->GetEnabled())continue;
			GatesEngine::GameObject* block, * playerBullet;
			block = b->GetGameObject();
			playerBullet = pb->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(b, pb))
			{
				block->Collision(pb);
				b->SetColor({ 1,0,0,0 });
				playerBullet->Collision(b);
				pb->SetColor({ 1,0,0,0 });
			}
		}

		{
			GatesEngine::GameObject* block, * cameraObject;
			block = b->GetGameObject();
			cameraObject = mainCameraCollider->GetGameObject();
			if (GatesEngine::CollisionManager::CheckAABB(b, mainCameraCollider))
			{
				block->Collision(mainCameraCollider);
				b->SetColor({ 1,0,0,0 });
				cameraObject->Collision(b);
				mainCameraCollider->SetColor({ 1,0,0,0 });
			}

		}
	}
}

void GameObjectCollisionManager::CheckCollisionHitEnemyTo()
{
	//敵とプレイヤー、玉
	for (auto& e : enemyColliders)
	{
		if (!e->GetEnabled())continue;
		for (auto& p : playerColliders)
		{
			if (!p->GetEnabled())continue;
			GatesEngine::GameObject* enemy, * player;
			enemy = e->GetGameObject();
			player = p->GetGameObject();

			if (e->GetType() == GatesEngine::ColliderType::CUBE)
			{
				if (GatesEngine::CollisionManager::CheckAABB(e, p))
				{
					enemy->Collision(p);
					e->SetColor({ 1,0,0,0 });
					player->Collision(e);
					p->SetColor({ 1,0,0,0 });
				}
			}
			else
			{
				if (GatesEngine::CollisionManager::CheckAABBToSphere(p, e))
				{
					enemy->Collision(p);
					e->SetColor({ 1,0,0,0 });
					player->Collision(e);
					p->SetColor({ 1,0,0,0 });
				}
			}
		}

		for (auto& pb : playerBulletColliders)
		{
			if (!pb->GetEnabled())continue;
			GatesEngine::GameObject* enemy, * playerBullet;
			enemy = e->GetGameObject();
			playerBullet = pb->GetGameObject();
			if (e->GetType() == GatesEngine::ColliderType::CUBE)
			{
				if (GatesEngine::CollisionManager::CheckAABB(e, pb))
				{
					enemy->Collision(pb);
					e->SetColor({ 1,0,0,0 });
					playerBullet->Collision(e);
					pb->SetColor({ 1,0,0,0 });
				}
			}
		}
	}
}

void GameObjectCollisionManager::CheckCollisionHitEnemyToCameraRay()
{
	if (!mainCamera)return;
	if (!playerBehaviour)return;
	//if (GatesEngine::Input::GetInstance()->GetMouse()->GetCheckHitButton(GatesEngine::MouseButtons::RIGHT_CLICK))
	//{
		for (auto& e : enemyColliders)
		{
			if (!e->GetEnabled())continue;
			GatesEngine::Math::Vector3 cameraDir = mainCamera->GetRotation().GetAxis().z;
			if (e->GetType() == GatesEngine::ColliderType::CUBE)
			{
				if (GatesEngine::CollisionManager::CheckAABBToRay(e, mainCamera->GetPosition(), cameraDir))
				{
					playerBehaviour->AddTarget(e->GetGameObject());
					e->SetColor({ 1,0,0,0 });
				}
			}
		}
	//}
}

void GameObjectCollisionManager::CheckCollisionHitBossTo()
{
	for (auto& pBullet : playerBulletColliders)
	{
		if (!pBullet->GetEnabled())continue;
		GatesEngine::GameObject* playerBullet, * boss;
		playerBullet = pBullet->GetGameObject();
		boss = bossCollider->GetGameObject();

		if (pBullet->GetType() == GatesEngine::ColliderType::CUBE)
		{
			if (GatesEngine::CollisionManager::CheckAABB(pBullet, bossCollider))
			{
				playerBullet->Collision(bossCollider);
				pBullet->SetColor({ 1,0,0,0 });
				boss->Collision(pBullet);
				bossCollider->SetColor({ 1,0,0,0 });
			}
		}
	}
}

void GameObjectCollisionManager::CheckCollisionHitEnemyBulletTo()
{
	for (auto& eb : enemyBulletCollider)
	{
		if (!eb->GetEnabled())continue;
		for (auto& p : playerColliders)
		{
			if (!p->GetEnabled())continue;

			GatesEngine::GameObject* enemyBullet, * player;
			enemyBullet = eb->GetGameObject();
			player = p->GetGameObject();

			if (GatesEngine::CollisionManager::CheckAABB(eb, p))
			{
				enemyBullet->Collision(p);
				eb->SetColor({ 1,0,0,0 });
				player->Collision(eb);
				p->SetColor({ 1,0,0,0 });
			}
		}

	}
}

void GameObjectCollisionManager::Update()
{
	CheckCollisionHitBlockTo();
	CheckCollisionHitEnemyTo();
	CheckCollisionHitEnemyToCameraRay();
	CheckCollisionHitBossTo();
	CheckCollisionHitEnemyBulletTo();
}

GatesEngine::Collider* GameObjectCollisionManager::AddCollider(GatesEngine::Collider* collider, GColliderType type)
{
	switch (type)
	{
	case GColliderType::BLOCK:
		blockColliders.push_back(collider);
		break;
	case GColliderType::ENEMY:
		enemyColliders.push_back(collider);
		break;
	case GColliderType::ENEMY_BULLET:
		enemyBulletCollider.push_back(collider);
		break;
	case GColliderType::PLAYER:
		playerColliders.push_back(collider);
		break;
	case GColliderType::PLAYER_BULLET:
		playerBulletColliders.push_back(collider);
		break;
	case GColliderType::PLAYER_CAMERA:
		mainCameraCollider = collider;
		break;
	case GColliderType::BOSS:
		bossCollider = collider;
		break;
	default:
		break;
	}

	return collider;
}

void GameObjectCollisionManager::SetCamera(GatesEngine::Camera3D* mainCamera)
{
	this->mainCamera = mainCamera;
}

void GameObjectCollisionManager::SetPlayerBehaviour(PlayerBehaviour* behaviour)
{
	playerBehaviour = behaviour;
}
