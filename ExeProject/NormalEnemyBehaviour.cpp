#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"

void NormalEnemyBehaviour::Start()
{
}

void NormalEnemyBehaviour::Update()
{
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();
}

void NormalEnemyBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}
