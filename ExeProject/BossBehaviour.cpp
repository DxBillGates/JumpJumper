#include "BossBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

void BossBehaviour::CallEnemy(const GatesEngine::Math::Vector3& centerPos, int count)
{
	int loopCount = 0;
	for (auto& e : normalEnemies)
	{
		if (loopCount >= count)break;
		if (!e)break;
		if (e->GetEnabled())continue;
		e->SetEnabled(true);
		e->Start();
		e->GetTransform()->position = centerPos;
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
		if (!e)break;
		e->SetEnabled(false);
	}

	int r = 2000;
	GatesEngine::Math::Vector3 pos;
	GatesEngine::Math::Vector3 offset;
	const int CALL_ENEMY = 10;
	for (int i = 0; i < CALL_ENEMY; ++i)
	{
		pos.x = r * sinf((2.0f * 3.1415f / CALL_ENEMY) * i) / 2 + offset.x;
		pos.y = 100;
		pos.z = r * cosf((2.0f * 3.1415f / CALL_ENEMY) * i) / 2 + offset.z;
		CallEnemy(pos, 1);
	}
}

void BossBehaviour::Update()
{
	if (!mainCamera)
	{
		mainCamera = dynamic_cast<GatesEngine::Camera3D*>(gameObject->GetGraphicsDevice()->GetMainCamera());
	}
}

void BossBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("testModel")->Draw();
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

		GatesEngine::Math::Vector3 offset = center + GatesEngine::Math::Vector3(0, gameObject->GetTransform()->scale.y * 2, 0);
		GatesEngine::Math::Vector3 spacePos = GatesEngine::Math::Vector3(0, 0, 0);
		GatesEngine::Math::Vector3 addPos = GatesEngine::Math::Vector3(size.x, 0, 0) * i;
		GatesEngine::Math::Vector3 fixPos = offset + addPos + spacePos - GatesEngine::Math::Vector3(size.x, 0, 0) * MAX_HP / 2;
		posMatrix = GatesEngine::Math::Matrix4x4::Translate(fixPos);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * rotateMatrix * posMatrix);
		mainCamera->Set(2);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,0,1),GatesEngine::Math::Vector4(0,0,0,1) });
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

void BossBehaviour::AddNormalEnemy(GatesEngine::GameObject* enemy)
{
	if (normalEnemyAddedCount > (int)normalEnemies.size())return;
	normalEnemies[normalEnemyAddedCount] = enemy;
	++normalEnemyAddedCount;
}
