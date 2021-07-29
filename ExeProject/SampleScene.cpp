#include "SampleScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"
#include "Header/GameObject/GameObject.h"
#include "PlayerBehaviour.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "PlayerSceneTransAnimation.h"
#include "Stage1.h"
#include "Stage2.h"
#include "Stage3.h"
#include <stdio.h>

SampleScene::SampleScene() :Scene()
{
}

SampleScene::SampleScene(const char* sceneName) : Scene(sceneName)
{
}

SampleScene::SampleScene(const char* sceneName, GatesEngine::Application* app) : Scene(sceneName, app)
{
	collisionManager.Initialize(6, { -10000 }, { 20000 });
	using namespace GatesEngine;
	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddBehavior<PlayerBehaviour>();
	g->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	collisionManager.AddColliderComponent(g->AddComponent<Collider>());
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	g->GetCollider()->SetSize({ 500 });
	g->SetName("player");

	auto* e1 = gameObjectManager.Add(new GameObject());
	e1->SetGraphicsDevice(graphicsDevice);
	e1->AddBehavior<NormalEnemyBehaviour>();
	collisionManager.AddColliderComponent(e1->AddComponent<Collider>());
	e1->SetCollider();
	e1->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e1->GetCollider()->SetSize({ 500 });
	e1->SetName("enemy1");
	e1->SetTag("enemy");
	e1->GetTransform()->position = { -300,0,300 };
}

SampleScene::~SampleScene()
{
}

void SampleScene::Initialize()
{
	gameObjectManager.Start();
}

void SampleScene::Update()
{
	gameObjectManager.Update();
	collisionManager.Update();
}

void SampleScene::Draw()
{
	using namespace GatesEngine::Math;

	gameObjectManager.Draw();

	graphicsDevice->ClearRenderTarget({ 135,206,235,0 },false);
	//ƒOƒŠƒbƒh•`‰æ
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, app->GetMainCamera()->GetData());

	for (int i = 0; i < 5; ++i)
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({0,-20000.0f * i,0}));
		graphicsDevice->GetMeshManager()->GetMesh("Grid")->Draw();
	}
	sceneTranslater.Draw(graphicsDevice);
}
