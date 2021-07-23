#include "..\..\Header\GameObject\GameObjectManager.h"

GatesEngine::GameObjectManager::GameObjectManager()
{
}

GatesEngine::GameObjectManager::~GameObjectManager()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		delete (*itr);
	}
}

GatesEngine::GameObject* GatesEngine::GameObjectManager::Add(GameObject* newGameObject)
{
	gameObjects.push_back(newGameObject);
	return newGameObject;
}

GatesEngine::GameObject* GatesEngine::GameObjectManager::Find(const char* name, const char* tag)
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		if ((*itr)->GetName() == name)
		{
			if (tag)
			{
				if ((*itr)->GetTag() == tag)
				{
					return (*itr);
				}
			}
			else
			{
				return (*itr);
			}
		}
	}
	return nullptr;
}

void GatesEngine::GameObjectManager::Start()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->Start();
	}
}

void GatesEngine::GameObjectManager::Update()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		if ((*itr)->GetEnabled())
		{
			(*itr)->Update();
		}

		for (auto other_itr = gameObjects.begin(); other_itr != gameObjects.end(); ++other_itr)
		{
			if ((*other_itr)->GetEnabled())
			{ 
				if (itr != other_itr)
				{
					Collider* mCollider = (*itr)->GetCollider();
					Collider* oCollider = (*other_itr)->GetCollider();

					if (mCollider && oCollider)
					{
						ColliderType mColliderType = mCollider->GetType();
						ColliderType oColliderType = oCollider->GetType();

						//‹…‚Æ‹…
						if (mColliderType == ColliderType::SPHERE && oColliderType == ColliderType::SPHERE)
						{
							float distance = Math::Vector3::Distance((*itr)->GetTransform()->position, (*other_itr)->GetTransform()->position);
							float r = (*itr)->GetCollider()->GetSize().x;
							if (distance * distance <= r * r)
							{
								(*itr)->Collision((*other_itr));
							}
						}

						//AABB
						if (mColliderType == ColliderType::CUBE && oColliderType == ColliderType::CUBE)
						{
							Math::Vector3 min1,min2,max1,max2;
							min1 = (*itr)->GetTransform()->position - mCollider->GetSize() / 2;
							max1 = (*itr)->GetTransform()->position + mCollider->GetSize() / 2;
							min2 = (*other_itr)->GetTransform()->position - oCollider->GetSize() / 2;
							max2 = (*other_itr)->GetTransform()->position + oCollider->GetSize() / 2;

							if (min1.x > max2.x)continue;
							if (max1.x < min2.x)continue;
							if (min1.y > max2.y)continue;
							if (max1.y < min2.y)continue;
							if (min1.z > max2.z)continue;
							if (max1.z < min2.z)continue;

							(*itr)->Collision((*other_itr));
						}
					}
				}
			}
		}
	}
}

void GatesEngine::GameObjectManager::Draw()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->Draw();
	}
}
