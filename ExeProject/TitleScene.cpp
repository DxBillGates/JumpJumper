#include "TitleScene.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Application/Application.h"
#include "TitleButtonUIBehaviour.h"
#include "DefaultGPUParticleEmitterBehaviour.h"
#include "RotateAxisBehaviour.h"

TitleScene::TitleScene()
	: TitleScene("TitleScene", nullptr)
{
}

TitleScene::TitleScene(const char* sceneName)
	: TitleScene(sceneName, nullptr)
{
}

TitleScene::TitleScene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, mainCamera(nullptr)
	, input(nullptr)
	, titleManager({})
	, black(1)
	, isSceneTransition(false)
	, isIncreaseBlack(true)
	, isDecreaseBlack(false)
	, sceneTransType()
{
	GatesEngine::Math::Vector2 windowSize = app->GetWindow()->GetWindowSize();

	GatesEngine::GameObject* object = nullptr;

	// タイトルで使うボタンを追加
	// Start,Exitの2つ
	GatesEngine::Math::Vector2 offset = { windowSize.x / 2 + windowSize.x / 3,windowSize.y / 2 + windowSize.y / 20 };
	GatesEngine::Math::Vector2 scale = { 64 * 6,64 * 2 };
	GatesEngine::Math::Vector2 space = { 0,300 };

	titleManager.SetInitialiseButtonSize(scale);
	ButtonUIType types[2];
	types[0] = ButtonUIType::START;
	types[1] = ButtonUIType::EXIT;
	GatesEngine::Texture* textures[2];
	textures[0] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("ui_start");
	textures[1] = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("ui_end");
	for (int i = 0; i < 2; ++i)
	{
		object = gameObjectManager.Add(new GatesEngine::GameObject());
		object->SetGraphicsDevice(graphicsDevice);
		auto* buttonUI = object->AddComponent<TitleButtonUIBehaviour>();
		buttonUI->SetPos({ offset + space * i });
		buttonUI->SetScale({ scale });
		buttonUI->SetType(types[i]);
		buttonUI->SetTexture(textures[i]);
		titleManager.AddButton(buttonUI);
	}


	//GPUパーティクル用のコンピュートシェーダー読み込み
	testCS = new GatesEngine::ComputePipeline(graphicsDevice, L"test");
	testCS->Create({ GatesEngine::RangeType::UAV,GatesEngine::RangeType::SRV });

	gpuParticleManager = graphicsDevice->GetParticleManager();
	// 演出用のパーティクルオブジェクト
	// デフォルトパーティクル + 軸回転オブジェクト作成
	{
		const int GAMEOBJECT_NUMBER = 50;
		for (int i = 0; i < GAMEOBJECT_NUMBER; ++i)
		{
			auto* gameObject = gameObjectManager.Add(new GatesEngine::GameObject());
			gameObject->SetGraphicsDevice(graphicsDevice);
			auto* emitter = gameObject->AddComponent<DefaultGPUParticleEmitterBehaviour>();
			emitter->SetComputeShader(testCS);
			emitter->CreateParticleEmitter(gpuParticleManager, 256);

			auto* rotateAxis = gameObject->AddComponent<RotateAxisBehaviour>();
		}
	}

	GatesEngine::Math::Vector2 renderTextureSize = app->GetWindow()->GetWindowSize();
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
	blurRenderTextures[4].Create(graphicsDevice, renderTextureSize / 6);
	blurRenderTextures[5].Create(graphicsDevice, renderTextureSize / 6);

	sceneRenderTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize(), { 141 / 2, 219 / 2, 228 / 2, 255 / 2 });
	sceneDepthTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize());
}

TitleScene::~TitleScene()
{
	delete testCS;
}

void TitleScene::Initialize()
{
	mainCamera = dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera());
	input = GatesEngine::Input::GetInstance();


	mainCamera->SetPosition({ 0,500,-0 });
	mainCamera->SetDirection({0,1,0});
	titleManager.SetGraphicsDevice(graphicsDevice);
	titleManager.Initialize();

	isSceneTransition = false;
	isIncreaseBlack = true;
	isDecreaseBlack = false;

	black = 0;

	gameObjectManager.Start();
}

void TitleScene::Update()
{
	gameObjectManager.Update();
	gpuParticleManager->Update();
	//if (input->GetKeyboard()->CheckHitKeyAll())
	//{
	//	app->GetSceneManager()->ChangeScene("Stage1Scene");
	//}
	//if (titleManager.GetIsExit())isTerminateApplication = true;
	if (black <= 0)
	{
		if (isDecreaseBlack)
		{
			isSceneTransition = true;
		}
	}
	if (isSceneTransition)
	{
		switch (sceneTransType)
		{
		case ButtonUIType::START:
			//app->GetSceneManager()->ChangeScene("SampleScene");
			app->GetSceneManager()->ChangeScene("Stage1Scene");
			app->Initialize();
			break;
		case ButtonUIType::EXIT:
			isTerminateApplication = true;
			break;
		default:
			break;
		}
	}

	const float PER_FRAME = 1.0f / 60.0f;
	const float INCREASE_TIME = 2;
	if (isIncreaseBlack)black += PER_FRAME / INCREASE_TIME;

	const float DECREASE_TIME = 2;
	if (isDecreaseBlack)black -= PER_FRAME / DECREASE_TIME;
	
	if (black >= 1)
	{
		titleManager.Update();
		black = 1;
		isIncreaseBlack = false;
		if (titleManager.GetClickButton())
		{
			ButtonUIType type = titleManager.GetClickButton()->GetType();
			switch (type)
			{
			case ButtonUIType::START:
				sceneTransType = ButtonUIType::START;
				isDecreaseBlack = true;
				break;
			case ButtonUIType::EXIT:
				sceneTransType = ButtonUIType::EXIT;
				isDecreaseBlack = true;
				break;
			default:
				break;
			}
		}
	}
}

void TitleScene::Draw()
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

	//GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	//GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	//GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	//graphicsDevice->ClearRenderTarget({ 141 / 2, 219 / 2, 228 / 2, 255 / 2 }, true, &sceneRenderTexture,&sceneDepthTexture);
	////graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

	//shaderManager->GetShader("Line")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	//mainCamera->Set(2);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	//meshManager->GetMesh("Grid")->Draw();

	//gameObjectManager.Draw();
}

void TitleScene::LateDraw()
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

	graphicsDevice->ClearRenderTarget({ 141/2, 219/2, 228/2, 255/2 }, true, &sceneRenderTexture, &sceneDepthTexture);
	shaderManager->GetShader("BloomShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920,1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,1080 / 2,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	subPostprocessTexture.Set(2);
	blurPlusParticleTex.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();
	gameObjectManager.LateDraw();
	titleManager.LateDraw();

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);
	GatesEngine::Math::Vector2 scale = { 1000,1000 };
	GatesEngine::Math::Vector2 pos = { 1920 / 2,1080 / 2 };

	shaderManager->GetShader("SceneTransitionFadeShader")->Set();
	scale = app->GetWindow()->GetWindowSize();
	pos = scale / 2;
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	float b = GatesEngine::Math::Easing::EaseInExpo(black);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(b, b, b, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	sceneRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();
	//GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	//GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	//GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();


	//graphicsDevice->ClearRenderTarget({ 141/2, 219/2, 228/2, 255/2 }, false, &sceneRenderTexture, &sceneDepthTexture);
	//gameObjectManager.SecondDraw();
	//gameObjectManager.LateDraw();
	//titleManager.LateDraw();

	//graphicsDevice->ClearRenderTarget({ 141 / 2, 219 / 2, 228 / 2, 255 / 2 }, true);
	//GatesEngine::Math::Vector2 scale = { 1000,1000 };
	//GatesEngine::Math::Vector2 pos = { 1920 / 2,1080 / 2 };

	//shaderManager->GetShader("SceneTransitionFadeShader")->Set();
	//scale = app->GetWindow()->GetWindowSize();
	//pos = scale / 2;
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	//float b = GatesEngine::Math::Easing::EaseInExpo(black);
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(b,b,b, 1));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//sceneRenderTexture.Set(3);
	//meshManager->GetMesh("2DPlane")->Draw();


}
