#include "TitleScene.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Application/Application.h"

TitleScene::TitleScene()
	: TitleScene("TitleScene",nullptr)
{
}

TitleScene::TitleScene(const char* sceneName)
	: TitleScene(sceneName,nullptr)
{
}

TitleScene::TitleScene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName,app)
	, mainCamera(nullptr)
{
}

void TitleScene::Initialize()
{
	mainCamera = app->GetMainCamera();
}

void TitleScene::Update()
{
}

void TitleScene::Draw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	shaderManager->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Translate(Vector3()));
	mainCamera->Set(2);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{Vector4(0,0,1,0),Vector4(1,1,1,1)});
	meshManager->GetMesh("Plane")->Draw();
}

void TitleScene::LateDraw()
{
}
