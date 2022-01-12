#include "TitleScene.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Application/Application.h"
#include "TitleButtonUIBehaviour.h"

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
	GatesEngine::Math::Vector2 scale = { 500,250 };
	GatesEngine::Math::Vector2 space = { 0,300 };

	titleManager.SetInitialiseButtonSize(scale);
	ButtonUIType types[2];
	types[0] = ButtonUIType::START;
	types[1] = ButtonUIType::EXIT;
	for (int i = 0; i < 2; ++i)
	{
		object = gameObjectManager.Add(new GatesEngine::GameObject());
		object->SetGraphicsDevice(graphicsDevice);
		auto* buttonUI = object->AddComponent<TitleButtonUIBehaviour>();
		buttonUI->SetPos({ offset + space * i });
		buttonUI->SetScale({ scale });
		buttonUI->SetType(types[i]);
		titleManager.AddButton(buttonUI);
	}

	sceneRenderTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize(), { 141, 219, 228, 255 });
	sceneDepthTexture.Create(graphicsDevice, app->GetWindow()->GetWindowSize());
}

void TitleScene::Initialize()
{
	mainCamera = dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera());
	input = GatesEngine::Input::GetInstance();


	mainCamera->SetPosition({ 0,500,-1000 });
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
			app->GetSceneManager()->ChangeScene("SampleScene");
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

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true, &sceneRenderTexture,&sceneDepthTexture);
	//graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

	shaderManager->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,0,0 }));
	mainCamera->Set(2);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(),GatesEngine::Math::Vector4() });
	meshManager->GetMesh("Grid")->Draw();

	gameObjectManager.Draw();
}

void TitleScene::LateDraw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();


	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, false, &sceneRenderTexture, &sceneDepthTexture);
	titleManager.LateDraw();
	gameObjectManager.LateDraw();

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);
	GatesEngine::Math::Vector2 scale = { 1000,1000 };
	GatesEngine::Math::Vector2 pos = { 1920 / 2,1080 / 2 };

	shaderManager->GetShader("SceneTransitionFadeShader")->Set();
	scale = app->GetWindow()->GetWindowSize();
	pos = scale / 2;
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	float b = GatesEngine::Math::Easing::EaseInExpo(black);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(b,b,b, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	sceneRenderTexture.Set(3);
	meshManager->GetMesh("2DPlane")->Draw();


}
