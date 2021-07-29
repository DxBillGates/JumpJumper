#include "Stage1Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"
#include "TrankingEnemyBehaviour.h"
#include "BlockBehaviour.h"

Stage1Scene::Stage1Scene() : Stage1Scene("Stage1Scene", nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName) : Stage1Scene(sceneName, nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, playerBehaviour(nullptr)
{
	collisionManager.Initialize(6, { -10000 }, { 20000 });
	using namespace GatesEngine;
	auto* gp = gameObjectManager.Add(new GameObject());
	gp->SetGraphicsDevice(graphicsDevice);
	playerBehaviour = gp->AddBehavior<PlayerBehaviour>();
	gp->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	collisionManager.AddColliderComponent(gp->AddComponent<Collider>());
	gp->SetCollider();
	gp->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	gp->GetCollider()->SetSize({ 50 });
	gp->SetName("player");

	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	auto* e = g->AddComponent<NormalEnemyBehaviour>();
	e->SetR(1000);
	e->SetY(0);
	collisionManager.AddColliderComponent(g->AddComponent<Collider>());
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 200 });
	g->SetTag("enemy");
	g->GetTransform()->position = { 1000,0,1000 };


	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	collisionManager.AddColliderComponent(g->AddComponent<Collider>());
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 500,10,500 });
	g->SetTag("block");
	g->GetTransform()->position = { 0,0,0 };

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
	collisionManager.Update();

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

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::UP))
	{
		for (int i = 0; i < 10; ++i)
		{
			using namespace GatesEngine;
			auto* g = gameObjectManager.Add(new GameObject());
			g->SetGraphicsDevice(graphicsDevice);
			g->AddComponent<BlockBehaviour>();
			collisionManager.AddColliderComponent(g->AddComponent<Collider>());
			g->SetCollider();
			g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
			g->GetCollider()->SetSize({ 100,10,100 });
			g->SetTag("block");
			float x, y, z;
			float range = 3000;
			x = ((float)std::rand() / RAND_MAX * range) - range / 2;
			y = ((float)std::rand() / RAND_MAX * range) - range / 2;
			z = ((float)std::rand() / RAND_MAX * range) - range / 2;
			g->GetTransform()->position = { x,y,z };
			g->GetTransform()->scale = g->GetCollider()->GetSize();
			g->Start();
		}
	}

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::DOWN))
	{
		float x, y, z;
		float range = 3000;
		x = ((float)std::rand() / RAND_MAX * range) - range / 2;
		y = ((float)std::rand() / RAND_MAX * range) - range / 2;
		z = ((float)std::rand() / RAND_MAX * range) - range / 2;
		for (int i = 0; i < 5; ++i)
		{
			for (int j = 0; j < 5; ++j)
			{
				using namespace GatesEngine;
				auto* g = gameObjectManager.Add(new GameObject());
				g->SetGraphicsDevice(graphicsDevice);
				g->AddComponent<BlockBehaviour>();
				collisionManager.AddColliderComponent(g->AddComponent<Collider>());
				g->SetCollider();
				g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
				g->GetCollider()->SetSize({ 50,10,50 });
				g->SetTag("block");
				g->GetTransform()->position = { x + 50 * i,y,z + 50 * j };
				g->GetTransform()->scale = g->GetCollider()->GetSize();
				g->Start();
			}
		}
	}
	static int i = 0;
	if (i % 120 == 0)
	{
		app->GetTimer()->SetIsShow(true);
	}
	else
	{
		app->GetTimer()->SetIsShow(false);
	}
	++i;
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