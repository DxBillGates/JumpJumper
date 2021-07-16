#include "TitleScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"
#include "Header/GameObject/GameObject.h"
#include "PlayerSceneTransAnimation.h"
#include <stdio.h>

TitleScene::TitleScene() :TitleScene("TitleScene", nullptr)
{
}

TitleScene::TitleScene(const char* sceneName) : TitleScene(sceneName, nullptr)
{
}

TitleScene::TitleScene(const char* sceneName, GatesEngine::Application* app)
	: Scene(sceneName, app)
	, titlePos(0, 1000, 500)
	, sceneTrasFlag(false)
	, sceneTrasTime(0)
{
	auto* g = gameObjectManager.Add(new GatesEngine::GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddBehavior<PlayerSceneTransAnimation>()->SetCamera(app->GetMainCamera());
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize()
{
	gameObjectManager.Start();
	sceneTrasFlag = false;
	sceneTrasTime = 0;
	GatesEngine::Camera* camera = app->GetMainCamera();
	camera->SetPosition({ 0,1000,0 });
	camera->SetYawPitch({ 0, GatesEngine::Math::ConvertToRadian(0) });
}

void TitleScene::Update()
{
	gameObjectManager.Update();

	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	{
		sceneTrasFlag = true;
	}

	if (sceneTrasFlag)sceneTrasTime += app->GetTimer()->GetElapsedTime();
	if (sceneTrasTime >= 2)
	{
		app->GetSceneManager()->ChangeScene("SampleScene");
		app->GetMainCamera()->SetPosition(GatesEngine::Math::Vector3(0, 10000, 0) + GatesEngine::Math::Vector3(0, 1000, -70));
		app->GetMainCamera()->Update();
	}
}

void TitleScene::Draw()
{
	gameObjectManager.Draw();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	GatesEngine::Math::Matrix4x4 titleMatrix = GatesEngine::Math::Matrix4x4::Scale({10,1,1}) * GatesEngine::Math::Matrix4x4::Translate(titlePos);
	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, titleMatrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2,app->GetMainCamera()->GetData());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();
}
