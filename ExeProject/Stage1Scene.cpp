#include "Stage1Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"
#include "TrankingEnemyBehaviour.h"
#include "BlockBehaviour.h"

Stage1Scene::Stage1Scene() : Scene("Stage1Scene", nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName) : Scene(sceneName, nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, playerBehaviour(nullptr)
{
	using namespace GatesEngine;
	auto* gp = gameObjectManager.Add(new GameObject());
	gp->SetGraphicsDevice(graphicsDevice);
	playerBehaviour = gp->AddBehavior<PlayerBehaviour>();
	gp->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	gp->AddComponent<Collider>();
	gp->SetCollider();
	gp->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	gp->GetCollider()->SetSize({ 50 });
	gp->SetName("player");

	//for (int i = 0; i < 10; ++i)
	//{
	//	g = gameObjectManager.Add(new GameObject());
	//	g->SetGraphicsDevice(graphicsDevice);
	//	g->AddComponent<BlockBehaviour>();
	//	g->AddComponent<Collider>();
	//	g->SetCollider();
	//	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	//	g->GetCollider()->SetSize({ 100 });
	//	g->SetTag("block");
	//	g->GetTransform()->position = { 0,0,(float)i * 100 };
	//}

	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	auto* e = g->AddComponent<NormalEnemyBehaviour>();
	e->SetR(1000);
	e->SetY(0);
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 200 });
	g->SetTag("enemy");
	g->GetTransform()->position = { 1000,0,1000 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	e = g->AddComponent<NormalEnemyBehaviour>();
	e->SetR(1500);
	e->SetY(500);
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 200 });
	g->SetTag("enemy");
	g->GetTransform()->position = { 1000,0,1000 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	e = g->AddComponent<NormalEnemyBehaviour>();
	e->SetR(1000);
	e->SetY(1500);
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 200 });
	g->SetTag("enemy");
	g->GetTransform()->position = { 1000,0,1000 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	auto* et = g->AddComponent<TrankingEnemyBehaviour>();
	et->SetTrakingGameObject(gp);
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 200 });
	g->SetTag("trak_enemy");
	g->GetTransform()->position = { 0,0,0 };

	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			g = gameObjectManager.Add(new GameObject());
			g->SetGraphicsDevice(graphicsDevice);
			g->AddComponent<BlockBehaviour>();
			g->AddComponent<Collider>();
			g->SetCollider();
			g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
			g->GetCollider()->SetSize({ 100 });
			g->SetTag("block");
			g->GetTransform()->position = { (float)j * 100 - 1000,3000,(float)i * 100 };
		}
	}

	for (int i = 0; i < 10; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			if (i == 0 || i == 9 || j == 0 || j == 9)
			//if(i == 4 || j == 4)
			{
				g = gameObjectManager.Add(new GameObject());
				g->SetGraphicsDevice(graphicsDevice);
				g->AddComponent<BlockBehaviour>();
				g->AddComponent<Collider>();
				g->SetCollider();
				g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
				g->GetCollider()->SetSize({ 100 });
				g->SetTag("block");
				g->GetTransform()->position = { (float)j * 100 - 450,(float)100 * (10 - i),(float)i * 100 - 450 };
			}
		}
	}
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

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, app->GetMainCamera()->GetData());

	for (int i = 0; i < 5; ++i)
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,-20000.0f * i,0 }));
		graphicsDevice->GetMeshManager()->GetMesh("Grid")->Draw();
	}
}
