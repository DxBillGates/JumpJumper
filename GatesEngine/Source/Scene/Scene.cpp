#include "..\..\Header\Scene\Scene.h"
#include "..\..\Header\Application\Application.h"
#include <stdio.h>

GatesEngine::Scene::Scene() :Scene("unnamedScene",nullptr)
{
#ifdef _DEBUG
	printf("–¼–³‚µ‚ÌScene¶¬\n");
#endif 
}

GatesEngine::Scene::Scene(const char* sceneName) :Scene(sceneName,nullptr)
{
#ifdef _DEBUG
	printf("%s¶¬\n", sceneName);
#endif 
}

GatesEngine::Scene::Scene(const char* sceneName, Application* app)
	: sceneName(sceneName)
	, changeNextSceneFlag(false)
	, app(app)
	, graphicsDevice(nullptr)
{
#ifdef _DEBUG
	printf("%s¶¬\n", sceneName);
#endif 
	graphicsDevice = app->GetGraphicsDevice();
}

GatesEngine::Scene::~Scene()
{
}

const char* GatesEngine::Scene::GetSceneName()
{
	return sceneName;
}

bool GatesEngine::Scene::IsChangeScene()
{
	return changeNextSceneFlag;
}

GatesEngine::GameObjectManager* GatesEngine::Scene::GetGameObjectManager()
{
	return &gameObjectManager;
}
