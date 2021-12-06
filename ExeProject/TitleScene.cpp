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
{
	GatesEngine::Math::Vector2 windowSize = app->GetWindow()->GetWindowSize();

	GatesEngine::GameObject* object = nullptr;

	// タイトルで使うボタンを追加
	// Start,Exitの2つ
	GatesEngine::Math::Vector2 offset = { windowSize.x / 2 + windowSize.x / 3,windowSize.y / 2 + windowSize.y / 20 };
	GatesEngine::Math::Vector2 scale = { 500,250 };
	GatesEngine::Math::Vector2 space = { 0,300 };
	for (int i = 0; i < 2; ++i)
	{
		object = gameObjectManager.Add(new GatesEngine::GameObject());
		object->SetGraphicsDevice(graphicsDevice);
		auto* buttonUI = object->AddComponent<TitleButtonUIBehaviour>();
		buttonUI->SetPos({ offset + space * i });
		buttonUI->SetScale({ scale });
	}
}

void TitleScene::Initialize()
{
	mainCamera = dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera());
	input = GatesEngine::Input::GetInstance();

	gameObjectManager.Start();
}

void TitleScene::Update()
{
	gameObjectManager.Update();
	if (input->GetKeyboard()->CheckHitKeyAll())
	{
		app->GetSceneManager()->ChangeScene("Stage1Scene");
	}
}

void TitleScene::Draw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, true);

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

	graphicsDevice->ClearRenderTarget({ 141, 219, 228, 255 }, false);

	gameObjectManager.LateDraw();
}
