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
		(*itr)->Update();
	}
}

void GatesEngine::GameObjectManager::Draw()
{
	for (auto itr = gameObjects.begin(); itr != gameObjects.end(); ++itr)
	{
		(*itr)->Draw();
	}
}
