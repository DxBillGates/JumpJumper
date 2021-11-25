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
	, input(nullptr)
{
}

void TitleScene::Initialize()
{
	mainCamera = dynamic_cast<GatesEngine::Camera3D*>(app->GetMainCamera());
	input = GatesEngine::Input::GetInstance();
}

void TitleScene::Update()
{
	if (input->GetKeyboard()->CheckHitKeyAll())
	{
		//app->GetSceneManager()->ChangeScene("Stage1Scene");
	}
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

	//shaderManager->GetShader("Line")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Translate(Vector3(0,100,0)));
	//meshManager->GetMesh("Line")->Draw();
}

void TitleScene::LateDraw()
{
	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	Vector3 cameraDir = mainCamera->GetRotation().GetAxis().z;
	Vector3 angleVector;
	angleVector.x = atan2f(-cameraDir.y, cameraDir.z);
	angleVector.y = atan2f(cameraDir.x, cameraDir.z);
	angleVector.z = atan2f(cameraDir.x, cameraDir.y);

	shaderManager->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, Matrix4x4::Scale({100, 100, 100}) * Matrix4x4::RotationZXY(angleVector) * Matrix4x4::Translate(Vector3(0, 100, 0)));
	mainCamera->Set(2);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ Vector4(0,0,1,0),Vector4(1,1,1,1) });
	meshManager->GetMesh("Line")->Draw();
}
