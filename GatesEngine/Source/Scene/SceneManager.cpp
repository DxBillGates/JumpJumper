#include "..\..\Header\Scene\SceneManager.h"

GatesEngine::SceneManager* GatesEngine::SceneManager::GetInstance()
{
	static SceneManager instance;
	return &instance;
}

void GatesEngine::SceneManager::AddScene(Scene* newScene)
{
	scenes.push_back(newScene);
}

void GatesEngine::SceneManager::ChangeScene(const char* sceneName)
{
	for (auto itr = scenes.begin(); itr != scenes.end(); ++itr)
	{
		if ((*itr)->GetSceneName() == sceneName)
		{
			beforeScene = currentScene;
			currentScene = (*itr);
			currentScene->Initialize();
			break;
		}
	}
}

void GatesEngine::SceneManager::ChangeSceneWithoutInitialize(const char* sceneName)
{
	for (auto itr = scenes.begin(); itr != scenes.end(); ++itr)
	{
		if ((*itr)->GetSceneName() == sceneName)
		{
			beforeScene = currentScene;
			currentScene = (*itr);
			break;
		}
	}
}

GatesEngine::Scene* GatesEngine::SceneManager::GetCurrentScene()
{
	return currentScene;
}

GatesEngine::Scene* GatesEngine::SceneManager::GetBeforeScene()
{
	return beforeScene;
}

GatesEngine::Scene* GatesEngine::SceneManager::GetScene(const char* sceneName)
{
	for (auto itr = scenes.begin(); itr != scenes.end(); ++itr)
	{
		if ((*itr)->GetSceneName() == sceneName)
		{
			return (*itr);
		}
	}
	return nullptr;
}

void GatesEngine::SceneManager::Initialize()
{
	if (currentScene != nullptr)
	{
		currentScene->Initialize();
	}
}

void GatesEngine::SceneManager::Update()
{
	if (currentScene != nullptr)
	{
		currentScene->Update();
	}
}

void GatesEngine::SceneManager::Draw()
{
	if (currentScene != nullptr)
	{
		currentScene->Draw();
	}
}

void GatesEngine::SceneManager::LateDraw()
{
	if (currentScene != nullptr)
	{
		currentScene->LateDraw();
	}
}

GatesEngine::SceneManager::SceneManager():currentScene(nullptr),beforeScene(nullptr)
{
}

GatesEngine::SceneManager::~SceneManager()
{
	for (auto itr = scenes.begin(); itr != scenes.end(); ++itr)
	{
		delete (*itr);
	}
}
