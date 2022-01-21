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
#include "DefaultGPUParticleEmitterBehaviour.h"
#include "ExplosionGPUParticleEmitterBehaviour.h"
#include "PlayerGPUParticleEmitterBehaviour.h"
#include "RotateAxisBehaviour.h"

float Stage1Scene::GaussFilter(const GatesEngine::Math::Vector2& pos, float value)
{
	return expf(-(pos.x * pos.x + pos.y * pos.y) / (2.0f * value * value));
}

void Stage1Scene::SetGaussFilterData(const GatesEngine::Math::Vector2& size, GatesEngine::Math::Vector2& dir, float gaussValue, GaussFilterData* data, int dataSize)
{
	GatesEngine::Math::Vector2 perPixel = { 1.0f / size.x,1.0f / size.y };

	data->data[0] = GatesEngine::Math::Vector4();
	data->data[0].z = GaussFilter({ 0,0 }, gaussValue);
	float totalWeight = data[0].data->z;

	for (int i = 1; i < 8; ++i)
	{
		data->data[i].x = dir.x * i * perPixel.x;
		data->data[i].y = dir.y * i * perPixel.y;
		data->data[i].z = GaussFilter(dir * (float)i, gaussValue);
		totalWeight += data->data[i].z * 2;
	}

	for (int i = 0; i < 8; ++i)
	{
		data->data[i].z /= totalWeight;
	}

	// 負の方向の値もセット
	for (int i = 8; i < 15; ++i)
	{
		data->data[i].x = -data->data[i - 7].x;
		data->data[i].y = -data->data[i - 7].y;
		data->data[i].z = data->data[i - 7].z;
	}
}

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
{
	//GPUパーティクル用のコンピュートシェーダー読み込み
	testCS = new GatesEngine::ComputePipeline(graphicsDevice, L"test");
	testCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

	testInitializeCS = new GatesEngine::ComputePipeline(graphicsDevice, L"testInitialize");
	testInitializeCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

	//GPUパーティクル用のマネージャー生成
	gpuParticleManager = graphicsDevice->GetParticleManager();

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
	auto* emitter = gp->AddComponent<PlayerGPUParticleEmitterBehaviour>();
	emitter->SetComputeShader(testCS);
	emitter->CreateParticleEmitter(gpuParticleManager, 256);
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
		auto* bulletBehaviour = bullet->AddBehavior<BulletBehaviour>();
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
	/*BossBehaviour* */bossBehaviour = b->AddBehavior<BossBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(b->AddComponent<Collider>()), GColliderType::BOSS);
	b->SetCollider();
	b->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	b->GetCollider()->SetSize({ 2 });
	b->GetTransform()->scale = 500;
	bossBehaviour->SetInitScale(500);
	b->GetTransform()->position = { 0,10000,0 };
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
	cameraBehaviour->SetPlayerObject(playerBehaviour->GetGameObject());
	//cameraBehaviour->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
	//g->GetTransform()->position = { 1000,1500,2000 };


	GatesEngine::Math::Vector2 renderTextureSize = app->GetWindow()->GetWindowSize();
	//renderTextureSize = {1280,720};

	shadowRenderTex.Create(graphicsDevice, renderTextureSize);
	shadowDepthTex.Create(graphicsDevice, renderTextureSize);
	resultRenderTex.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(1, 1, 1, 255));
	resultDepthTex.Create(graphicsDevice, renderTextureSize);
	lateDrawResultRenderTex.Create(graphicsDevice, renderTextureSize, GatesEngine::Math::Vector4(141 / 2, 219 / 2, 228 / 2, 255));
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

	GatesEngine::Math::Vector2 right, up;
	right = { 1,0 };
	up = { 0,1 };
	float gaussValue = 2.5f;
	SetGaussFilterData(renderTextureSize / 2, right, gaussValue, &gaussData[0], 0);
	SetGaussFilterData(renderTextureSize / 2, up, gaussValue, &gaussData[1], 0);
	gaussValue *= 2;
	SetGaussFilterData(renderTextureSize / 4, right, gaussValue, &gaussData[2], 0);
	SetGaussFilterData(renderTextureSize / 4, up, gaussValue, &gaussData[3], 0);
	gaussValue *= 2;
	SetGaussFilterData(renderTextureSize / 8, right, gaussValue, &gaussData[4], 0);
	SetGaussFilterData(renderTextureSize / 8, up, gaussValue, &gaussData[5], 0);

	blurRenderTextures[0].Create(graphicsDevice, renderTextureSize / 2);
	blurRenderTextures[1].Create(graphicsDevice, renderTextureSize / 2);
	blurRenderTextures[2].Create(graphicsDevice, renderTextureSize / 4);
	blurRenderTextures[3].Create(graphicsDevice, renderTextureSize / 4);
	blurRenderTextures[4].Create(graphicsDevice, renderTextureSize / 6);
	blurRenderTextures[5].Create(graphicsDevice, renderTextureSize / 6);

	reverceResoTextures[0].Create(graphicsDevice, renderTextureSize / 8);
	reverceResoTextures[1].Create(graphicsDevice, renderTextureSize / 4);
	reverceResoTextures[2].Create(graphicsDevice, renderTextureSize / 4);


	for (int i = 0; i < 10; ++i)
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
		secondCollider->SetSize(50);
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
		auto* bulletBehaviour = bullet->AddBehavior<BulletBehaviour>();
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

		//e->SetTarget(playerBehaviour->GetGameObject());
		e->AddBullet(bulletBehaviour);

		bullet->Start();
		bullet->GetTransform()->position = g->GetTransform()->position;

		enemyManager.RegisterEnemy(e, g);
	}

	//gpuParticleCS = new GatesEngine::ComputePipeline(graphicsDevice, L"GPUParticle");
	//gpuParticleCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV,GatesEngine::RangeType::SRV });

	sceneRenderTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize(), { 141, 219, 228, 255 });
	sceneDepthTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize());

	// デフォルトパーティクルオブジェクト作成
	{
		auto* gameObject = gameObjectManager.Add(new GatesEngine::GameObject());
		gameObject->SetGraphicsDevice(graphicsDevice);
		gameObject->SetName("explosionParticleEmitter");
		gameObject->SetEnabled(false);
		auto* emitter = gameObject->AddComponent<ExplosionGPUParticleEmitterBehaviour>();
		emitter->SetComputeShader(testCS);
		emitter->CreateParticleEmitter(gpuParticleManager, 256);
	}

	// デフォルトパーティクル + 軸回転オブジェクト作成
	{
		const int GAMEOBJECT_NUMBER = 50;
		for (int i = 0; i < GAMEOBJECT_NUMBER; ++i)
		{
			auto* gameObject = gameObjectManager.Add(new GatesEngine::GameObject());
			gameObject->SetGraphicsDevice(graphicsDevice);
			auto* emitter = gameObject->AddComponent<DefaultGPUParticleEmitterBehaviour>();
			emitter->SetComputeShader(testCS);
			emitter->CreateParticleEmitter(gpuParticleManager, 256 * 5);

			auto* rotateAxis = gameObject->AddComponent<RotateAxisBehaviour>();
			rotateAxis->SetCenter(boss->GetTransform()->position);

			rotateAxisParticleManager.Add(rotateAxis, emitter);
		}
	}
}

Stage1Scene::~Stage1Scene()
{
	delete testCS;
	delete testInitializeCS;
	//delete gpuParticleManager;
}

void Stage1Scene::Initialize()
{
	isSceneTransition = false;
	isIncreaseBlack = true;
	isDecreaseBlack = false;
	black = 0;

	enemyManager.Initialize();
	gameState.Initialize();
	gameObjectManager.Start();
	stage.GetCollisionManager()->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
	//app->GetTimer()->SetFrameRate(60);
	battleCount = 1;
	rotateAxisParticleManager.Initialize();
}

void Stage1Scene::Update()
{
	bool preBossIsDead = bossBehaviour->GetIsDead();
	bool preIsEndparticle = rotateAxisParticleManager.GetIsEndParticles(bossBehaviour->GetGameObject()->GetTransform()->position);
	BossState preBossState = bossBehaviour->GetState();
	gameState.Update(app->GetTimer()->GetElapsedTime());
	enemyManager.Update(battleCount);
	gameObjectManager.Update();
	////八分木空間分割すり抜けバグ多発したため一旦なし
	//collisionManager.Update();
	stage.Update();

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::Y))
	{
		gameState.ChangeState();
	}

	//if (bossBehaviour->GetHp() <= 0 && bossBehaviour->GetState() == BossState::NONE)
	//{
	//	if (gameState.GetCurrentState() == GameState::BOSS_BATTLE || gameState.GetCurrentState() == GameState::FISRT_BOSS_BATTLE)
	//	{
	//		gameState.ChangeState();
	//		bossBehaviour->SetBossState(BossState::LEFT);
	//		battleCount++;
	//	}
	//}

	if (!preIsEndparticle && rotateAxisParticleManager.GetIsEndParticles(bossBehaviour->GetGameObject()->GetTransform()->position))
	{
		//rotateAxisParticleManager.EndParticles();
		rotateAxisParticleManager.ReverseParticle();
		bossBehaviour->SetBossState(BossState::JOIN);
		gameState.ChangeState();
	}

	if (preBossState == BossState::JOIN && bossBehaviour->GetState() == BossState::HEALING)
	{
		rotateAxisParticleManager.EndParticles();
	}

	if (enemyManager.IsDestroyAllGroup())
	{
		//bossBehaviour->SetBossState(BossState::JOIN);
		rotateAxisParticleManager.StartParticles(bossBehaviour->GetGameObject()->GetTransform()->position);
		//gameState.ChangeState();
	}


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
			auto* bulletBehaviour = bullet->AddBehavior<BulletBehaviour>();
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
			//boss->GetComponent<BossBehaviour>()->AddNormalEnemy(g, e);

			enemyManager.RegisterEnemy(e, g);
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
	gpuParticleManager->Update();

	if (!preBossIsDead && bossBehaviour->GetIsDead())
	{
		GatesEngine::GameObject* gameObject = gameObjectManager.Find("explosionParticleEmitter");
		gameObject->Start();
		gameObject->GetTransform()->position = boss->GetTransform()->position;
	}

	bool preIsDecreaseBlack = isDecreaseBlack;
	isDecreaseBlack = bossBehaviour->GetIsEndScaleAnimation();
	if (!preIsDecreaseBlack && isDecreaseBlack)
	{
		GatesEngine::GameObject* gameObject = gameObjectManager.Find("explosionParticleEmitter");
		gameObject->SetEnabled(false);
	}
	if (black <= 0)
	{
		if (isDecreaseBlack)
		{
			isSceneTransition = true;
		}
	}
	if (isSceneTransition)
	{
		app->GetSceneManager()->ChangeScene("TitleScene");
		app->Initialize();
	}

	const float PER_FRAME = 1.0f / 60.0f;
	const float INCREASE_TIME = 2;
	if (isIncreaseBlack)black += PER_FRAME / INCREASE_TIME;

	const float DECREASE_TIME = 6;
	if (isDecreaseBlack)black -= PER_FRAME / DECREASE_TIME;

	if (black >= 1)
	{
		black = 1;
		isIncreaseBlack = false;
	}
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
	graphicsDevice->ClearRenderTarget(GatesEngine::Math::Vector4(141/2, 219/2, 228/2, 255), true, &lateDrawResultRenderTex, &lateDrawResultDepthTex);
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
	shaderManager->GetShader("DefaultMeshShader")->Set();

	mainCamera->Set(2);
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
	color = { 0.4f,1,1,1 };
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

	for (int i = 0; i < 6; ++i)
	{
		graphicsDevice->ClearRenderTarget({ 0,0,0,1 }, true, &blurRenderTextures[i]);
		shaderManager->GetShader("GaussBlurShader")->Set();

		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, gaussData[i]);
		// 偶数なら輝度抽出テクスチャをセット
		if (i % 2 == 0)
		{
			if (i == 0)
			{
				brightnessTexture.Set(3);
			}
			else
			{
				blurRenderTextures[i - 1].Set(3);
			}
		}
		// 奇数なら横ブラーした結果をセット
		else
		{
			blurRenderTextures[i - 1].Set(3);
		}

		meshManager->GetMesh("2DPlane")->Draw();
	}

	//graphicsDevice->ClearRenderTarget({ 0,0,0,1 }, true, &reverceResoTextures[0]);
	//shaderManager->GetShader("TextureSpriteShader")->Set();

	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//blurRenderTextures[5].Set(3);

	//meshManager->GetMesh("2DPlane")->Draw();

	//graphicsDevice->ClearRenderTarget({ 0,0,0,1 }, true, &reverceResoTextures[1]);
	//shaderManager->GetShader("TextureSpriteShader")->Set();

	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//reverceResoTextures[0].Set(3);

	//meshManager->GetMesh("2DPlane")->Draw();

	//graphicsDevice->ClearRenderTarget({ 0,0,0,1 }, true, &reverceResoTextures[2]);
	//shaderManager->GetShader("TextureSpriteShader")->Set();

	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//blurRenderTextures[3].Set(3);

	//meshManager->GetMesh("2DPlane")->Draw();


	//graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurRenderTexture);

	//shaderManager->GetShader("HGaussBlurShader")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//brightnessTexture.Set(3);
	//meshManager->GetMesh("2DPlane")->Draw();

	//graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurRenderTexture2);

	//shaderManager->GetShader("HGaussBlurShader")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//blurRenderTexture.Set(3);
	//meshManager->GetMesh("2DPlane")->Draw();

	//graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurRenderTexture3);

	//shaderManager->GetShader("HGaussBlurShader")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//blurRenderTexture2.Set(3);
	//meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurPlusParticleTex);
	shaderManager->GetShader("BloomShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	redrawRenderTexture.Set(2);
	blurRenderTextures[1].Set(3);
	blurRenderTextures[3].Set(4);
	blurRenderTextures[5].Set(5);
	meshManager->GetMesh("2DPlane")->Draw();

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true, & sceneRenderTexture, & sceneDepthTexture);
	shaderManager->GetShader("BloomShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 })* GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	subPostprocessTexture.Set(2);
	blurPlusParticleTex.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();


	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);
	GatesEngine::Math::Vector2 scale = { 1000,1000 };
	GatesEngine::Math::Vector2 pos = { 1920 / 2,1080 / 2 };

	shaderManager->GetShader("SceneTransitionFadeShader")->Set();
	scale = app->GetWindow()->GetWindowSize();
	pos = scale / 2;
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 })* GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	float b = GatesEngine::Math::Easing::EaseInExpo(black);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(b, b, b, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	sceneRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();
	//gameObjectManager.LateDraw();
	//gpuParticleEmitter.Draw(app->GetMainCamera(), testCS, 1000);
}
