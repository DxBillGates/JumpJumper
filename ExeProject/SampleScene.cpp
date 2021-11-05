#include "SampleScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"
#include <stdio.h>

SampleScene::SampleScene() :Scene()
{
}

SampleScene::SampleScene(const char* sceneName) : Scene(sceneName)
{
}

SampleScene::SampleScene(const char* sceneName, GatesEngine::Application* app) : Scene(sceneName, app)
{
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	gameObjectManager.Start();
}

void SampleScene::Update()
{
	gameObjectManager.Update();
	//collisionManager.Update();
}

void SampleScene::Draw()
{
}
