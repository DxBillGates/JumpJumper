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
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

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

	testInitializeCS = new GatesEngine::ComputePipeline(graphicsDevice, L"testInitialize");
	testInitializeCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

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
	gp->SetTag("player");

	for (int i = 0; i < 20; ++i)
	{
		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<PlayerBulletBehaviour>();
		auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
		bulletBehaviour->SetGPUParticleEmitter(gParticleEmitter);
		gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 256);
		gParticleEmitter->SetComputeShader(testCS);
		gParticleEmitter->SetInitializeShader(testInitializeCS);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(bullet->AddComponent<Collider>()), GColliderType::PLAYER_BULLET);
		bullet->SetCollider();
		bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		bullet->GetCollider()->SetSize({ 1 });
		bullet->GetTransform()->scale = 10;
		bullet->SetTag("playerBullet");
		bullet->SetName("playerBullet");

		playerBehaviour->AddBullet(bulletBehaviour);
	}

	auto* b = gameObjectManager.Add(new GameObject());
	b->SetGraphicsDevice(graphicsDevice);
	BossBehaviour* bossBehaviour = b->AddBehavior<BossBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(b->AddComponent<Collider>()), GColliderType::BOSS);
	b->SetCollider();
	b->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	b->GetCollider()->SetSize({ 2 });
	b->GetTransform()->scale = 500;
	b->GetTransform()->position = { 0,1000,-5000 };
	b->SetTag("Boss");
	boss = b;

	GatesEngine::GameObject* g = nullptr;


	g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddComponent<BlockBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::BLOCK);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 1 });
	g->GetTransform()->scale = { 10000,1000,10000 };
	g->SetTag("block");
	g->GetTransform()->position = { 0,-600,0 };

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


	GatesEngine::Math::Vector2 renderTextureSize = app->GetWindow()->GetWindowSize();
	//renderTextureSize = {1280,720};

	shadowRenderTex.Create(graphicsDevice, renderTextureSize);
	shadowDepthTex.Create(graphicsDevice, renderTextureSize);
	resultRenderTex.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(1, 1, 1, 255));
	resultDepthTex.Create(graphicsDevice, renderTextureSize);
	lateDrawResultRenderTex.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(141, 219, 228, 255));
	lateDrawResultDepthTex.Create(graphicsDevice, renderTextureSize);
	resultRenderShadowTex.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(1, 1, 1, 255));
	parlinNoiseTex.Create(graphicsDevice, renderTextureSize);
	parlinNoiseHeightMapTex.Create(graphicsDevice, renderTextureSize);
	subPostprocessTexture.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(1, 1, 1, 255));
	brightnessTexture.Create(graphicsDevice, renderTextureSize);
	blurRenderTexture.Create(graphicsDevice, renderTextureSize / 8);
	blurRenderTexture2.Create(graphicsDevice, renderTextureSize / 4);
	blurRenderTexture3.Create(graphicsDevice, renderTextureSize / 8);
	redrawRenderTexture.Create(graphicsDevice, renderTextureSize);
	redrawDepthTex.Create(graphicsDevice, renderTextureSize);
	blurPlusParticleTex.Create(graphicsDevice, renderTextureSize);



	for (int i = 0; i < 20; ++i)
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

		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<PlayerBulletBehaviour>();
		auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
		bulletBehaviour->SetGPUParticleEmitter(gParticleEmitter);
		gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 1280);
		gParticleEmitter->SetComputeShader(testCS);
		gParticleEmitter->SetInitializeShader(testInitializeCS);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(bullet->AddComponent<Collider>()), GColliderType::ENEMY_BULLET);
		bullet->AddComponent<Collider>();
		bullet->SetCollider();
		bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		bullet->GetCollider()->SetSize({ 1 });
		bullet->GetTransform()->scale = 10;
		bullet->SetTag("enemyBullet");
		bullet->SetName("enemyBullet");

		e->SetTarget(playerBehaviour->GetGameObject());
		e->AddBullet(bulletBehaviour);

		bullet->Start();
		bullet->GetTransform()->position = g->GetTransform()->position;

		enemyManager.RegisterEnemy(e,g);
	}
}

Stage1Scene::~Stage1Scene()
{
	delete testCS;
	delete testInitializeCS;
	delete gpuParticleManager;
}

void Stage1Scene::Initialize()
{
	gameObjectManager.Start();
	stage.GetCollisionManager()->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
	//app->GetTimer()->SetFrameRate(60);
	battleCount = 1;
}

void Stage1Scene::Update()
{
	gpuParticleEmitter.Update();
	enemyManager.Update(battleCount);

	if (enemyManager.IsDestroyAllGroup())battleCount++;
	gameObjectManager.Update();
	////八分木空間分割すり抜けバグ多発したため一旦なし
	//collisionManager.Update();
	stage.Update();


	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::DOWN))
	{
		using namespace GatesEngine;
		float x, y, z;
		float range = 3000;
		x = Random::Rand(-range, range);
		y = Random::Rand(-100, range);
		z = Random::Rand(-range, range);

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

			auto* bullet = gameObjectManager.Add(new GameObject());
			bullet->SetGraphicsDevice(graphicsDevice);
			auto* bulletBehaviour = bullet->AddBehavior<PlayerBulletBehaviour>();
			auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
			bulletBehaviour->SetGPUParticleEmitter(gParticleEmitter);
			gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 1280);
			gParticleEmitter->SetComputeShader(testCS);
			gParticleEmitter->SetInitializeShader(testInitializeCS);
			stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(bullet->AddComponent<Collider>()), GColliderType::ENEMY_BULLET);
			bullet->AddComponent<Collider>();
			bullet->SetCollider();
			bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
			bullet->GetCollider()->SetSize({ 1 });
			bullet->GetTransform()->scale = 10;
			bullet->SetTag("enemyBullet");
			bullet->SetName("enemyBullet");

			e->SetTarget(playerBehaviour->GetGameObject());
			e->AddBullet(bulletBehaviour);

			bullet->Start();
			bullet->GetTransform()->position = g->GetTransform()->position;
			boss->GetComponent<BossBehaviour>()->AddNormalEnemy(g, e);

			enemyManager.RegisterEnemy(e,g);
		}
	}

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::I) || playerBehaviour->GetHp() <= 0)
	{
		app->GetSceneManager()->ChangeScene("TitleScene");
		app->Initialize();
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
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();
	GatesEngine::SceneManager* sceneManager = app->GetSceneManager();
	GatesEngine::Camera* mainCamera = app->GetMainCamera();

	graphicsDevice->GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();

	//GatesEngine::ResourceManager::GetShaderManager()->GetShader("CreateParlinNoiseTextureShader")->Set();
	////graphicsDevice.ClearRenderTargetOutDsv({ 0,0,0,1 }, true, &parlinNoiseTex);
	//graphicsDevice->SetMultiRenderTarget({ &parlinNoiseTex,&parlinNoiseHeightMapTex }, nullptr, { 0,0,0,1 });
	//using namespace GatesEngine::Math;
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 1920,1080,0 }) * Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, Vector4(1));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, Vector4(app->GetTimer()->GetElapsedApplicationTime()));
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();

	//シャドウマップ用深度描画
	graphicsDevice->ClearRenderTarget({ 0,0,0,1 }, true, &shadowRenderTex, &shadowDepthTex);
	shaderManager->GetShader("DefaultMeshShader")->Set();

	//プレイヤーの上空から一度描画
	GatesEngine::GameObject* player = sceneManager->GetCurrentScene()->GetGameObjectManager()->Find("player");
	GatesEngine::Math::Vector3 pos = (player) ? player->GetTransform()->position : GatesEngine::Math::Vector3();
	GatesEngine::B2 lightViewData;
	float angle = 45;
	GatesEngine::Math::Vector3 dir = GatesEngine::Math::Vector3(0, 0, 1).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	GatesEngine::Math::Vector3 up = GatesEngine::Math::Vector3(0, 1, 0).Normalize() * GatesEngine::Math::Matrix4x4::RotationX(GatesEngine::Math::ConvertToRadian(angle));
	lightViewData.viewMatrix = GatesEngine::Math::Matrix4x4::GetViewMatrixLookTo({ pos - dir * 10000 }, dir, up);
	lightViewData.projMatrix = GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 20000,20000 }, 1, 20000);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, lightViewData);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,-1,0,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });

	//シーンの描画
	gameObjectManager.Draw();

	graphicsDevice->SetMultiRenderTarget({ &resultRenderTex,&resultRenderShadowTex }, &resultDepthTex, GatesEngine::Math::Vector4(1, 1, 1, 255));

	//using namespace GatesEngine::Math;
	//GatesEngine::ResourceManager::GetShaderManager()->GetShader("Texture")->Set();
	//graphicsDevice.GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 100 }) * Matrix4x4::RotationX(ConvertToRadian(-90)) * Matrix4x4::Translate({ 0,5000,1000 }));
	//mainCamera->Set(2);
	////graphicsDevice.GetCBufferAllocater()->BindAndAttach(4, Vector4(timer.GetElapsedApplicationTime()/10));
	//parlinNoiseTex.Set(5);
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();

	//深度テクスチャを利用してプレイヤー視点で描画
	shaderManager->GetShader("testMultiRTVShader")->Set();

	mainCamera->Set(2);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,1,0).Normalize(),GatesEngine::Math::Vector4(1,1,1,1) });
	GatesEngine::Math::Matrix4x4 lightViewMatrix = lightViewData.viewMatrix * lightViewData.projMatrix;
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);

	//シーンの描画
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(4, lightViewMatrix);
	shadowDepthTex.Set(5);

	gameObjectManager.Draw();
}

void Stage1Scene::LateDraw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();
	GatesEngine::SceneManager* sceneManager = app->GetSceneManager();
	GatesEngine::Camera* mainCamera = app->GetMainCamera();

	//グリッドの描画
	graphicsDevice->ClearRenderTarget(GatesEngine::Math::Vector4(141, 219, 228, 255), true, &lateDrawResultRenderTex, &lateDrawResultDepthTex);
	shaderManager->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	meshManager->GetMesh("Grid")->Draw();

	//スプライトやコライダーのワイヤーフレーム表示
	gameObjectManager.LateDraw();
	/*sceneManager->LateDraw();*/
	graphicsDevice->GetCBVSRVUAVHeap()->Set();
	//using namespace GatesEngine::Math;
	//auto* input = GatesEngine::Input::GetInstance();
	//if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::P))
	//{
	//	shaderManager->GetShader("TestTesselationShader")->Set(true);
	//}
	//else
	//{
	//	shaderManager->GetShader("TestTesselationShader")->Set(false);
	//}
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({ 100 }) * Matrix4x4::RotationX(ConvertToRadian(-90)) * Matrix4x4::Translate({ 0,5000,1000 }));
	//mainCamera->Set(2);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(4, GatesEngine::B3{ Vector4(),Vector4(0,10000,10000,1) });
	//parlinNoiseTex.Set(5);
	//parlinNoiseHeightMapTex.Set(6);

	//meshManager->GetMesh("DividePlane")->Draw();

	////描画結果から深度テクスチャを利用してアウトライン付与してを描画
	//graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

	////sceneManager->Draw();
	////sceneManager->LateDraw();
	//shaderManager->GetShader("PostEffect_OutlineShader")->Set();
	//graphicsDevice->GetCBVSRVUAVHeap()->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//static GatesEngine::Math::Vector4 color = { 0,0,0,1 };
	////if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D1))color = { 1,0,0,1 };
	////if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D2))color = { 0,1,0,1 };
	////if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D3))color = { 0,0,1,1 };
	////if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D4))color = { 0,0,0,1 };
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, color);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//resultDepthTex.Set(3);
	//resultRenderTex.Set(4);
	//lateDrawResultDepthTex.Set(5);
	//lateDrawResultRenderTex.Set(6);
	//resultRenderShadowTex.Set(7);
	//meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &redrawRenderTexture, &redrawDepthTex);
	gameObjectManager.Draw();
	gameObjectManager.SecondDraw();


	graphicsDevice->ClearRenderTarget({ 1,1,1, 255 }, true, &subPostprocessTexture);
	shaderManager->GetShader("PostEffect_OutlineShader")->Set();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	GatesEngine::Math::Vector4 color = { 0,0,0,1 };
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, color);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	resultDepthTex.Set(3);
	resultRenderTex.Set(4);
	lateDrawResultDepthTex.Set(5);
	lateDrawResultRenderTex.Set(6);
	resultRenderShadowTex.Set(7);
	meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &brightnessTexture);
	shaderManager->GetShader("BrightnessSamplingShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	color = { 0.2f,1,1,1 };
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, color);
	//subPostprocessTexture.Set(3);
	//resultRenderTex.Set(3);
	//particleRenderTexture.Set(4);
	//resultRenderShadowTex.Set(4);
	//lateDrawResultRenderTex.Set(4);
	redrawRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

	//graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

	//shaderManager->GetShader("TextureSpriteShader")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//color = { 0,0,0,1 };
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, color);
	//subPostprocessTexture.Set(3);
	////resultRenderTex.Set(3);
	////redrawRenderTexture.Set(3);
	//brightnessTexture[0].Set(3);
	//meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurRenderTexture);

	shaderManager->GetShader("HGaussBlurShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	brightnessTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, & blurRenderTexture2);

	shaderManager->GetShader("HGaussBlurShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 })* GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	blurRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, & blurRenderTexture3);

	shaderManager->GetShader("HGaussBlurShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 })* GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	blurRenderTexture2.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurPlusParticleTex);
	shaderManager->GetShader("BloomShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 })* GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	redrawRenderTexture.Set(2);
	blurRenderTexture3.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

	static bool flag = false;
	if(app->GetInput()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D1))
	flag = !flag;
	if (flag)
	{
		graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

		shaderManager->GetShader("BloomShader")->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		subPostprocessTexture.Set(2);
		blurPlusParticleTex.Set(3);
		meshManager->GetMesh("2DPlane")->Draw();
	}
	else
	{
		graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

		shaderManager->GetShader("BloomShader")->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		subPostprocessTexture.Set(2);
		blurPlusParticleTex.Set(3);
		meshManager->GetMesh("2DPlane")->Draw();
	}



	//gameObjectManager.LateDraw();
	//gpuParticleEmitter.Draw(app->GetMainCamera(), testCS, 1000);
}
