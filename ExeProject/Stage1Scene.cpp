#include "Stage1Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"
#include "TrankingEnemyBehaviour.h"
#include "BlockBehaviour.h"
#include "PlayerBulletBehaviour.h"

Stage1Scene::Stage1Scene() : Stage1Scene("Stage1Scene", nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName) : Stage1Scene(sceneName, nullptr)
{
}

Stage1Scene::Stage1Scene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, playerBehaviour(nullptr)
	, testCS(nullptr)
	, gpuParticleManager(nullptr)
	, gpuParticleEmitter({})
{
	testCS = new GatesEngine::ComputePipeline(graphicsDevice, L"test");
	testCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

	gpuParticleManager = new GatesEngine::GPUParticleManager(graphicsDevice);
	gpuParticleEmitter.Create(gpuParticleManager, 10000);

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

	for (int i = 0; i < 20; ++i)
	{
		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<PlayerBulletBehaviour>();
		collisionManager.AddColliderComponent(bullet->AddComponent<Collider>());
		bullet->SetCollider();
		bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		bullet->GetCollider()->SetSize({ 10 });
		bullet->SetName("playerBullet");

		playerBehaviour->AddBullet(bulletBehaviour);
	}

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
	g->GetTransform()->position = { 1000,200,1000 };


	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	collisionManager.AddColliderComponent(g->AddComponent<Collider>());
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 10000,1,10000 });
	g->SetTag("block");
	g->GetTransform()->position = { 0,-10,0 };

}

Stage1Scene::~Stage1Scene()
{
	delete testCS;
	delete gpuParticleManager;
}

void Stage1Scene::Initialize()
{
	gameObjectManager.Start();
	sceneTranslater.SetTranslateState(SceneTranslater::TranslateState::DOWN);
	sceneTranslater.StartSceneTranslate(1);
}

void Stage1Scene::Update()
{
	gpuParticleEmitter.Update();
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
			g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
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
				g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
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
}

void Stage1Scene::LateDraw()
{
	gameObjectManager.LateDraw();
	//gpuParticleEmitter.Draw(app->GetMainCamera(), testCS,1000);
}
