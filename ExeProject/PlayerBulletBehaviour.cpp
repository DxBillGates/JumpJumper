#include "PlayerBulletBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

PlayerBulletBehaviour::PlayerBulletBehaviour()
{
}

PlayerBulletBehaviour::~PlayerBulletBehaviour()
{
}

void PlayerBulletBehaviour::Start()
{
	//gameObject->GetTransform()->scale = gameObject->GetCollider()->GetSize() - 5;
	Initialize();
}

void PlayerBulletBehaviour::Update()
{
	if (isUse)
	{
		if (!isHoming)
		{
			vel += shotVector;
			gameObject->GetTransform()->position += vel * 10;
		}
		else
		{
			GatesEngine::Math::Vector3 diff = target - gameObject->GetTransform()->position;
			GatesEngine::Math::Vector3 acc;
			acc += (diff - vel * homingTime) * 2.0f / (homingTime * homingTime);

			homingTime -= 0.016f / 2.0f;

			vel += acc * 0.016f / 2.0f;
			gameObject->GetTransform()->position += vel * 0.016f / 2.0f;
		}
	}
	else
	{
		gameObject->GetTransform()->position = setPos;
	}
	PlayerBullet::Update();
}

void PlayerBulletBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Sphere")->Draw();
}

void PlayerBulletBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}

void PlayerBulletBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == "enemy")
	{
		Initialize();
	}
	//if (otherCollider->GetGameObject()->GetTag() == "block")
	//{
	//	isUse = false;
	//	Initialize();
	//}
}
