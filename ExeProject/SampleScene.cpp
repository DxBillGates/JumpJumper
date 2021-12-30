#include "SampleScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "PlayerCameraBehaviour.h"
#include "PlayerBulletBehaviour.h"
#include "GPUParticleEmitterBehaviour.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Util/Random.h"
#include "BlockBehaviour.h"
#include <stdio.h>

SampleScene::SampleScene() :Scene()
{
}

SampleScene::SampleScene(const char* sceneName) : Scene(sceneName)
{
}

SampleScene::SampleScene(const char* sceneName, GatesEngine::Application* app) : Scene(sceneName, app)
{
	tutorialSystem = new TutorialSystem(graphicsDevice);

	gpuParticleManager = graphicsDevice->GetParticleManager();
	//GPUパーティクル用のコンピュートシェーダー読み込み
	testCS = new GatesEngine::ComputePipeline(graphicsDevice, L"test");
	testCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

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
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 2, right, gaussValue, &gaussData[0], 0);
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 2, up, gaussValue, &gaussData[1], 0);
	gaussValue *= 2;
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 4, right, gaussValue, &gaussData[2], 0);
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 4, up, gaussValue, &gaussData[3], 0);
	gaussValue *= 2;
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 8, right, gaussValue, &gaussData[4], 0);
	GatesEngine::Math::SetGaussFilterData(renderTextureSize / 8, up, gaussValue, &gaussData[5], 0);

	blurRenderTextures[0].Create(graphicsDevice, renderTextureSize / 2);
	blurRenderTextures[1].Create(graphicsDevice, renderTextureSize / 2);
	blurRenderTextures[2].Create(graphicsDevice, renderTextureSize / 4);
	blurRenderTextures[3].Create(graphicsDevice, renderTextureSize / 4);
	blurRenderTextures[4].Create(graphicsDevice, renderTextureSize / 8);
	blurRenderTextures[5].Create(graphicsDevice, renderTextureSize / 8);

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

	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	auto* cameraBehaviour = g->AddComponent<PlayerCameraBehaviour>();
	stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::PLAYER_CAMERA);
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
	g->GetCollider()->SetSize({ 2 });
	g->SetTag("mainCamera");
	cameraBehaviour->SetPlayerObject(playerBehaviour->GetGameObject());

	for (int i = 0; i < 20; ++i)
	{
		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<BulletBehaviour>();
		auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
		bulletBehaviour->SetGPUParticleEmitter(gParticleEmitter);
		gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 256);
		gParticleEmitter->SetComputeShader(testCS);
		//gParticleEmitter->SetInitializeShader(testInitializeCS);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(bullet->AddComponent<Collider>()), GColliderType::PLAYER_BULLET);
		bullet->SetCollider();
		bullet->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		bullet->GetCollider()->SetSize({ 1 });
		bullet->GetTransform()->scale = 10;
		bullet->SetTag("playerBullet");
		bullet->SetName("playerBullet");

		playerBehaviour->AddBullet(bulletBehaviour);
	}

	for (int i = 0; i < 1; ++i)
	{
		auto* g = gameObjectManager.Add(new GameObject());
		g->SetGraphicsDevice(graphicsDevice);
		auto* e = g->AddComponent<NormalEnemyBehaviour>();
		//e->SetBoss(boss);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(g->AddComponent<Collider>()), GColliderType::ENEMY);
		g->SetCollider();
		g->GetCollider()->SetType(GatesEngine::ColliderType::CUBE);
		g->GetTransform()->scale = { 100 };
		g->GetCollider()->SetSize(2);

		auto* secondCollider = g->AddComponent<Collider>();
		secondCollider->SetType(GatesEngine::ColliderType::SPHERE);
		secondCollider->SetSize(100);
		stage.GetCollisionManager()->AddCollider(collisionManager.AddColliderComponent(secondCollider), GColliderType::ENEMY);

		g->SetTag("enemy");
		float x, y, z;
		float range = 3000;
		x = Random::Rand(-range, range);
		y = Random::Rand(-100, range);
		z = Random::Rand(-range, range);
		g->GetTransform()->position = { -1000,0,0 };
		g->Start();

		auto* bullet = gameObjectManager.Add(new GameObject());
		bullet->SetGraphicsDevice(graphicsDevice);
		auto* bulletBehaviour = bullet->AddBehavior<BulletBehaviour>();
		auto* gParticleEmitter = bullet->AddBehavior<GPUParticleEmitterBehaviour>();
		bulletBehaviour->SetGPUParticleEmitter(gParticleEmitter);
		gParticleEmitter->CreateParticleEmitter(gpuParticleManager, 1280);
		gParticleEmitter->SetComputeShader(testCS);
		//gParticleEmitter->SetInitializeShader(testInitializeCS);
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

		//enemyManager.RegisterEnemy(e, g);
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
	g->GetTransform()->position = { 0,-600,0 };
}

SampleScene::~SampleScene()
{
	delete tutorialSystem;
	delete testCS;
}

void SampleScene::Initialize()
{
	tutorialSystem->Initialize();
	gameObjectManager.Start();
	stage.GetCollisionManager()->SetCamera(dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera()));
}

void SampleScene::Update()
{
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::UP))
	{
		tutorialSystem->AddClearCount();
	}
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::DOWN))
	{
		tutorialSystem->TransCurrentState();
	}
	tutorialSystem->Update();
	stage.Update();
	gpuParticleManager->Update();
	gameObjectManager.Update();
}

void SampleScene::Draw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();
	GatesEngine::SceneManager* sceneManager = app->GetSceneManager();
	GatesEngine::Camera* mainCamera = app->GetMainCamera();

	graphicsDevice->GetCBufferAllocater()->ResetCurrentUseNumber();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();

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

void SampleScene::LateDraw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();
	GatesEngine::SceneManager* sceneManager = app->GetSceneManager();
	GatesEngine::Camera* mainCamera = app->GetMainCamera();

	//グリッドの描画
	graphicsDevice->ClearRenderTarget(GatesEngine::Math::Vector4(141 / 2, 219 / 2, 228 / 2, 255), true, &lateDrawResultRenderTex, &lateDrawResultDepthTex);
	shaderManager->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	meshManager->GetMesh("Grid")->Draw();

	//スプライトやコライダーのワイヤーフレーム表示
	gameObjectManager.LateDraw();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();

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
	redrawRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();

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

	graphicsDevice->ClearRenderTarget({ 0,0,0, 255 }, true, &blurPlusParticleTex);
	shaderManager->GetShader("BloomShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	redrawRenderTexture.Set(2);
	blurRenderTextures[1].Set(3);
	blurRenderTextures[3].Set(4);
	blurRenderTextures[5].Set(5);
	meshManager->GetMesh("2DPlane")->Draw();

	static bool flag = false;
	if (app->GetInput()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D1))
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

	tutorialSystem->Draw();

}
