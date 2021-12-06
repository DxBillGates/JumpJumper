#include "BossBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"

void BossBehaviour::CallEnemy(const GatesEngine::Math::Vector3& centerPos, int count)
{
	GatesEngine::Math::Vector3 left, right;
	left = { -1000,0,0 };
	right = { 1000,0,0 };
	int loopCount = 0;
	int flag;
	for (auto& e : normalEnemies)
	{
		if (loopCount >= count)break;
		if (!e.gameObject)break;
		if (e.gameObject->GetEnabled())continue;
		e.gameObject->SetEnabled(true);
		e.gameObject->Start();
		e.enemyBehaviour->SetTarget(centerPos, { 0,10000,0 });
		e.enemyBehaviour->SetTime(GatesEngine::Random::Rand(2,3));

		flag = ((int)GatesEngine::Random::Rand(-10, 10));
		GatesEngine::Math::Vector3 offset = (flag > 0) ? left : right;
		e.enemyBehaviour->SetPosition(gameObject->GetTransform()->position + offset);
		e.gameObject->GetTransform()->position = gameObject->GetTransform()->position + offset;
		++loopCount;
	}
}

BossBehaviour::BossBehaviour()
	: MAX_HP(10)
	, hp(MAX_HP)
	, mainCamera(nullptr)
	, callEnemyFlag(false)
	, MAX_NORMAL_ENEMY(100)
	, normalEnemyAddedCount(0)
	, callEnemyInterval(0)
	, n(0)
{
	normalEnemies.resize(MAX_NORMAL_ENEMY);
}

BossBehaviour::~BossBehaviour()
{
}

void BossBehaviour::Start()
{
	for (auto& e : normalEnemies)
	{
		if (!e.gameObject)break;
		e.gameObject->SetEnabled(false);
	}
//
//	int r = 5000;
//	GatesEngine::Math::Vector3 pos;
//	GatesEngine::Math::Vector3 offset;
//	const int CALL_ENEMY = 10;
//	for (int i = 0; i < CALL_ENEMY; ++i)
//	{
//		pos.x = r * sinf((2.0f * 3.1415f / CALL_ENEMY) * i) / 2 + offset.x;
//		pos.y = 100;
//		pos.z = r * cosf((2.0f * 3.1415f / CALL_ENEMY) * i) / 2 + offset.z;
//		CallEnemy(pos, 1);
//	}
}

void BossBehaviour::Update()
{
	if (!mainCamera)
	{
		mainCamera = dynamic_cast<GatesEngine::Camera3D*>(gameObject->GetGraphicsDevice()->GetMainCamera());
	}
	const int MAX_TIME = 3;
	if (callEnemyInterval >= MAX_TIME)
	{
		int r = 5000;
		GatesEngine::Math::Vector3 pos;
		GatesEngine::Math::Vector3 offset;
		const int CALL_ENEMY = 10;
		if (n > CALL_ENEMY)n = 0;
		pos.x = r * sinf((2.0f * 3.1415f / CALL_ENEMY) * n) / 2 + offset.x;
		pos.y = 100;
		pos.z = r * cosf((2.0f * 3.1415f / CALL_ENEMY) * n) / 2 + offset.z;
		CallEnemy(pos, 1);
		++n;
		callEnemyInterval = 0;
	}

	callEnemyInterval += 0.016f / 2.0f;
}

void BossBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("testModel")->Draw();


	GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(500);
	GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(1000, 0, 0));

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();


	posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(-1000, 0, 0));

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();
}

void BossBehaviour::OnLateDraw()
{
	if (!mainCamera)return;
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	GatesEngine::Math::Vector3 center = gameObject->GetTransform()->position;

	GatesEngine::Math::Vector3 size = 200;
	GatesEngine::Math::Quaternion rotate = GatesEngine::Math::Quaternion({ {0,0,1},0 });
	GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(size);
	GatesEngine::Math::Matrix4x4 rotateMatrix = GatesEngine::Math::Quaternion::Rotation(rotate) * mainCamera->GetRotation();
	GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(center);

	for (int i = 0; i < hp; ++i)
	{
		GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultMeshShader")->Set();

		graphicsDevice->GetCBVSRVUAVHeap()->Set();
		GatesEngine::Math::Vector3 offset = center + GatesEngine::Math::Vector3(0, gameObject->GetTransform()->scale.y * 2, 0);
		GatesEngine::Math::Vector3 spacePos = GatesEngine::Math::Vector3(0, 0, 0);
		GatesEngine::Math::Vector3 addPos = GatesEngine::Math::Vector3(size.x, 0, 0) * i;
		GatesEngine::Math::Vector3 fixPos = offset + addPos + spacePos - GatesEngine::Math::Vector3(size.x, 0, 0) * MAX_HP / 2;
		posMatrix = GatesEngine::Math::Matrix4x4::Translate(fixPos);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * rotateMatrix * posMatrix);
		mainCamera->Set(2);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,-1,1),GatesEngine::Math::Vector4(1,0,0,1) });
		GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();
	}
}

void BossBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	GatesEngine::GameObject* other = otherCollider->GetGameObject();
	if (other->GetTag() == "enemy")
	{
		--hp;
	}
}

void BossBehaviour::AddNormalEnemy(GatesEngine::GameObject* enemy, Enemy* enemyBehaviour)
{
	if (normalEnemyAddedCount > (int)normalEnemies.size())return;
	normalEnemies[normalEnemyAddedCount].enemyBehaviour = enemyBehaviour;
	normalEnemies[normalEnemyAddedCount].gameObject = enemy;
	++normalEnemyAddedCount;
}
