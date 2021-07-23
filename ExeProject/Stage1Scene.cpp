#include "Stage1Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"

Stage1Scene::Stage1Scene() : Scene("Stage1Scene",nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName) : Scene(sceneName,nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, playerBehaviour(nullptr)
{
	using namespace GatesEngine;
	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	playerBehaviour = g->AddBehavior<PlayerBehaviour>();
	g->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	g->GetCollider()->SetSize({ 50 });
	g->SetName("player");

	auto* e1 = gameObjectManager.Add(new GameObject());
	e1->SetGraphicsDevice(graphicsDevice);
	e1->AddBehavior<NormalEnemyBehaviour>();
	e1->AddComponent<Collider>();
	e1->SetCollider();
	e1->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e1->GetCollider()->SetSize({ 50 });
	e1->SetName("enemy1");
	e1->SetTag("enemy");
	e1->GetTransform()->position = { -300,0,300 };
}

Stage1Scene::~Stage1Scene()
{
}

void Stage1Scene::Initialize()
{
	gameObjectManager.Start();
	sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::DOWN);
	sceneTranslater.StartSceneTranslate(1);
}

void Stage1Scene::Update()
{
	gameObjectManager.Update();

	SceneTranslater::TranslateState sceneTranslaterState = sceneTranslater.GetTranslateState();
	sceneTranslater.Update(app->GetTimer()->GetElapsedTime());

	if (playerBehaviour)
	{
		if (playerBehaviour->GetKillCount() >= stage.GetNeedKillCount())
		{
			playerBehaviour->ResetKillCount();
			sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::UP);
			sceneTranslater.StartSceneTranslate(1);
		}
	}

	if (sceneTranslaterState == SceneTranslater::TranslateState::UP && sceneTranslaterState != sceneTranslater.GetTranslateState())
	{
		app->GetSceneManager()->ChangeScene("SelectScene");
	}
}

void Stage1Scene::Draw()
{
	gameObjectManager.Draw();
	sceneTranslater.Draw(graphicsDevice);
}
