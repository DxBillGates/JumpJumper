#include "Stage1Scene.h"
#include "Header/Application/Application.h"
#include "NormalEnemyBehaviour.h"
#include "BlockBehaviour.h"
#include "BossBehaviour.h"
#include "PlayerBehaviour.h"
#include "PlayerBulletBehaviour.h"
#include "GPUParticleEmitterBehaviour.h"
#include "PlayerCameraBehaviour.h"
#include "Header/Util/Random.h"

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
	//GPUパーティクル用のコンピュートシェーダー読み込み
	testCS = new GatesEngine::ComputePipeline(graphicsDevice, L"test");
	testCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

	//GPUパーティクル用のマネージャー生成
	gpuParticleManager = new GatesEngine::GPUParticleManager(graphicsDevice);
	//GPUパーティクルマネージャーからパーティクル1万個確保
	gpuParticleEmitter.Create(gpuParticleManager, 1000);

	////八分木空間分割すり抜けバグ多発したため一旦なし
	//collisionManager.Initialize(5, { -10000,-100,-10000 }, { 10000 });

	//テスト用オブジェクト配置
	using namespace GatesEngine;
	auto* gp = gameObjectManager.Add(new GameObject());
	gp->SetGraphicsDevice(graphicsDevice);
	playerBehaviour = gp->AddBehavior<PlayerBehaviour>();
	//gp->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(gp->AddComponent<Collider>()), GColliderType::PLAYER);
	stage.GetCollisionManager()->SetPlayerBehaviour(playerBehaviour);
	gp->SetCollider();
	gp->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	gp->GetCollider()->SetSize({ 1 });
	gp->GetTransform()->scale = 50;
	gp->SetName("player");

	for (int i = 0; i < 20; ++i)
	{
		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<PlayerBulletBehaviour>();
		auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
		gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 256);
		gParticleEmitter->SetComputeShader(testCS);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(bullet->AddComponent<Collider>()), GColliderType::PLAYER_BULLET);
		bullet->SetCollider();
		bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		bullet->GetCollider()->SetSize({ 1 });
		bullet->GetTransform()->scale = 10;
		bullet->SetTag("playerBullet");
		bullet->SetName("playerBullet");

		playerBehaviour->AddBullet(bulletBehaviour);
	}

	gp = gameObjectManager.Add(new GameObject());
	gp->SetGraphicsDevice(graphicsDevice);
	BossBehaviour* bossBehaviour = gp->AddBehavior<BossBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(gp->AddComponent<Collider>()), GColliderType::BOSS);
	gp->SetCollider();
	gp->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	gp->GetCollider()->SetSize({ 2 });
	gp->GetTransform()->scale = 500;
	gp->GetTransform()->position = { 0,1000,-5000 };
	gp->SetTag("Boss");
	boss = gp;

	GatesEngine::GameObject* g = nullptr;
	//auto* g = gameObjectManager.Add(new GameObject());
	for (int i = 0; i < 5; ++i)
	{
		for (int j = 0; j < 10; ++j)
		{
			g = gameObjectManager.Add(new GameObject());
			//auto* g = gameObjectManager.Add(new GameObject());
			g->SetGraphicsDevice(graphicsDevice);
			auto* e = g->AddComponent<NormalEnemyBehaviour>();
			e->SetBoss(boss);
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::ENEMY);
			g->SetCollider();
			g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
			g->GetCollider()->SetSize({ 2 });
			auto* secondCollider = g->AddComponent<Collider>();
			secondCollider->SetType(GatesEngine::ColliderType::SPHERE);
			secondCollider->SetSize(10);
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(secondCollider), GColliderType::ENEMY);
			g->GetTransform()->scale = 100;
			g->SetTag("enemy");
			g->GetTransform()->position = { (float)i * 250,(float)(j + 1) * 250,1000 };

			bossBehaviour->AddNormalEnemy(g,e);
		}
	}


	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 1 });
	g->GetTransform()->scale = { 10000,1000,10000 };
	g->SetTag("block");
	g->GetTransform()->position = { 0,-500,0 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 1 });
	g->GetTransform()->scale = { 500,100,500 };
	g->SetTag("block");
	g->GetTransform()->position = { 2000,1000,0 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 1 });
	g->GetTransform()->scale = { 500,100,500 };
	g->SetTag("block");
	g->GetTransform()->position = { -2000,1000,0 };

	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	auto* cameraBehaviour = g->AddComponent<PlayerCameraBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::PLAYER_CAMERA);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 2 });
	//g->GetTransform()->scale = { 500,100,500 };
	g->SetTag("mainCamera");
	//cameraBehaviour->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
	//g->GetTransform()->position = { 1000,1500,2000 };

}

Stage1Scene::~Stage1Scene()
{
	delete testCS;
	delete gpuParticleManager;
}

void Stage1Scene::Initialize()
{
	gameObjectManager.Start();
	stage.GetCollisionManager()->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
	//app->GetTimer()->SetFrameRate(60);
}

void Stage1Scene::Update()
{
	gpuParticleEmitter.Update();
	gameObjectManager.Update();
	////八分木空間分割すり抜けバグ多発したため一旦なし
	//collisionManager.Update();
	stage.Update();

	//テスト用オブジェクト配置
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::UP))
	{
		using namespace GatesEngine;
		for (int i = 0; i < 3; ++i)
		{
			auto* g = gameObjectManager.Add(new GameObject());
			g->SetGraphicsDevice(graphicsDevice);
			g->AddComponent<BlockBehaviour>();
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
			//複数のコライダーに対応済み
			auto* c = stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
			c->SetPosition({ 0,100,100 });
			c = stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
			c->SetPosition({ 0,200,200 });
			g->SetCollider();
			g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
			g->GetTransform()->scale = { 500,100,500 };
			g->SetTag("block");
			float x, y, z;
			float range = 3000;
			x = Random::Rand(-range, range);
			y = Random::Rand(-100, range);
			z = Random::Rand(-range, range);
			g->GetTransform()->position = { x,y,z };
			g->Start();
		}
	}

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::DOWN))
	{
		using namespace GatesEngine;
		float x, y, z;
		float range = 3000;
		x = Random::Rand(-range, range);
		y = Random::Rand(-100, range);
		z = Random::Rand(-range, range);
		//for (int i = 0; i < 5; ++i)
		//{
		//	for (int j = 0; j < 5; ++j)
		//	{
		//		auto* g = gameObjectManager.Add(new GameObject());
		//		g->SetGraphicsDevice(graphicsDevice);
		//		g->AddComponent<BlockBehaviour>();
		//		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
		//		g->SetCollider();
		//		g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		//		g->GetTransform()->scale = { 50,10,50 };
		//		g->SetTag("block");
		//		g->GetTransform()->position = { x + 50 * i,y,z + 50 * j };
		//		g->Start();
		//	}
		//}

		for (int i = 0; i < 1; ++i)
		{
			auto* g = gameObjectManager.Add(new GameObject());
			g->SetGraphicsDevice(graphicsDevice);
			auto* e = g->AddComponent<NormalEnemyBehaviour>();
			e->SetBoss(boss);
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::ENEMY);
			g->SetCollider();
			g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
			g->GetTransform()->scale = { 100 };
			g->GetCollider()->SetSize(2);

			auto* secondCollider = g->AddComponent<Collider>();
			secondCollider->SetType(GatesEngine::ColliderType::SPHERE);
			secondCollider->SetSize(10);
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(secondCollider), GColliderType::ENEMY);

			g->SetTag("enemy");
			float x, y, z;
			float range = 3000;
			x = Random::Rand(-range, range);
			y = Random::Rand(-100, range);
			z = Random::Rand(-range, range);
			g->GetTransform()->position = { x,y,z };
			g->Start();
		}
	}

	//FPS表示120フレームに一度表示
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
}

void Stage1Scene::LateDraw()
{
	gameObjectManager.LateDraw();
	//gpuParticleEmitter.Draw(app->GetMainCamera(), testCS, 1000);
}
