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
		if (!(*itr)->GetEnabled())continue;
		(*itr)->Start();
	}
}

void GatesEngine::GameObjectManager::Update()
{
	auto begin = gameObjects.begin();
	auto end = gameObjects.end();

	for (auto itr = begin; itr != end; ++itr)
	{
		if ((*itr)->GetEnabled())
		{
			(*itr)->Update();
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

void GatesEngine::GameObjectManager::SecondDraw()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->SecondDraw();
	}
}

void GatesEngine::GameObjectManager::LateDraw()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->LateDraw();
	}
}