#include "PlayerBulletBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"

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

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	graphicsDevice->GetShaderManager()->GetShader("MeshShadowShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetMeshManager()->GetMesh("Sphere")->Draw();
}

void PlayerBulletBehaviour::OnCollision(GatesEngine::GameObject* other)
{
	if (other->GetTag() == "enemy")
	{
		isUse = false;
	}

}
