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

						if (mColliderType == ColliderType::SPHERE && oColliderType == ColliderType::SPHERE)
						{
							float distance = Math::Vector3::Distance((*itr)->GetTransform()->position, (*other_itr)->GetTransform()->position);
							float r = (*itr)->GetCollider()->GetSize().x;
							if (distance * distance <= r * r)
							{
								(*itr)->Collision((*other_itr));
							}
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
