#include "Stage2Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"

Stage2Scene::Stage2Scene()
	: Scene("Stage2Scene")
{
}

Stage2Scene::Stage2Scene(const char* sceneName)
	: Scene(sceneName,nullptr)
{
}

Stage2Scene::Stage2Scene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName,app)
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

	auto* e2 = gameObjectManager.Add(new GameObject());
	e2->SetGraphicsDevice(graphicsDevice);
	e2->AddBehavior<NormalEnemyBehaviour>();
	e2->AddComponent<Collider>();
	e2->SetCollider();
	e2->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e2->GetCollider()->SetSize({ 50 });
	e2->SetName("enemy2");
	e2->SetTag("enemy");
	e2->GetTransform()->position = { 300,0,300 };

	//auto* e3 = gameObjectManager.Add(new GameObject());
	//e3->SetGraphicsDevice(graphicsDevice);
	//e3->AddBehavior<NormalEnemyBehaviour>();
	//e3->AddComponent<Collider>();
	//e3->SetCollider();
	//e3->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	//e3->GetCollider()->SetSize({ 50 });
	//e3->SetName("enemy3");
	//e3->SetTag("enemy");
	//e3->GetTransform()->position = { 300,0,-300 };
}

Stage2Scene::~Stage2Scene()
{
}

void Stage2Scene::Initialize()
{
	gameObjectManager.Start();
	sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::DOWN);
	sceneTranslater.StartSceneTranslate(1);
}

void Stage2Scene::Update()
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

void Stage2Scene::Draw()
{
	gameObjectManager.Draw();
	sceneTranslater.Draw(graphicsDevice);
}
