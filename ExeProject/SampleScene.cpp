#include "SampleScene.h"
#include "Header/Application/Application.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"
#include "Header/GameObject/GameObject.h"
#include "PlayerBehaviour.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "PlayerSceneTransAnimation.h"
#include <stdio.h>

SampleScene::SampleScene() :Scene()
{
}

SampleScene::SampleScene(const char* sceneName) : Scene(sceneName)
{
}

SampleScene::SampleScene(const char* sceneName, GatesEngine::Application* app) : Scene(sceneName, app)
{
	using namespace GatesEngine;
	auto* g = gameObjectManager.Add(new GameObject());
	g->SetGraphicsDevice(graphicsDevice);
	g->AddBehavior<PlayerBehaviour>();
	g->GetComponent<PlayerBehaviour>()->SetCamera(app->GetMainCamera());
	g->AddComponent<Collider>();
	g->SetCollider();
	g->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	g->GetCollider()->SetSize({ 50 });

	auto* e1 = gameObjectManager.Add(new GameObject());
	e1->SetGraphicsDevice(graphicsDevice);
	e1->AddBehavior<NormalEnemyBehaviour>();
	e1->AddComponent<Collider>();
	e1->SetCollider();
	e1->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e1->GetCollider()->SetSize({ 50 });
	e1->SetName("enemy1");
	e1->SetTag("enemy");
	e1->GetTransform()->position = { -300,0,300 };

	auto* e2 = gameObjectManager.Add(new GameObject());
	e2->SetGraphicsDevice(graphicsDevice);
	e2->AddBehavior<NormalEnemyBehaviour>();
	e2->AddComponent<Collider>();
	e2->SetCollider();
	e2->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e2->GetCollider()->SetSize({ 50 });
	e2->SetName("enemy2");
	e2->SetTag("enemy");
	e2->GetTransform()->position = { 300,0,300 };

	auto* e3 = gameObjectManager.Add(new GameObject());
	e3->SetGraphicsDevice(graphicsDevice);
	e3->AddBehavior<NormalEnemyBehaviour>();
	e3->AddComponent<Collider>();
	e3->SetCollider();
	e3->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e3->GetCollider()->SetSize({ 50 });
	e3->SetName("enemy3");
	e3->SetTag("enemy");
	e3->GetTransform()->position = { 300,0,-300 };

	auto* e4 = gameObjectManager.Add(new GameObject());
	e4->SetGraphicsDevice(graphicsDevice);
	e4->AddBehavior<NormalEnemyBehaviour>();
	e4->AddComponent<Collider>();
	e4->SetCollider();
	e4->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e4->GetCollider()->SetSize({ 50 });
	e4->SetName("enemy4");
	e4->SetTag("enemy");
	e4->GetTransform()->position = { -300,0,-300 };

	auto* e5 = gameObjectManager.Add(new GameObject());
	e5->SetGraphicsDevice(graphicsDevice);
	e5->AddBehavior<NormalEnemyBehaviour>();
	e5->AddComponent<Collider>();
	e5->SetCollider();
	e5->GetCollider()->SetType(GatesEngine::ColliderType::SPHERE);
	e5->GetCollider()->SetSize({ 50 });
	e5->SetName("enemy4");
	e5->SetTag("enemy");
	e5->GetTransform()->position = { 0,0,0 };
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

}
