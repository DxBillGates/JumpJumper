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
		gameObject->GetTransform()->position += vel * 10;
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
	if (other->GetTag() == "enemy")
	{
		isUse = false;
	}

}
