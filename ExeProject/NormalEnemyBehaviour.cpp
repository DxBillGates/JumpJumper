#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"

void NormalEnemyBehaviour::Start()
{
	time = 0;
	delay = (float)(rand() % 10 + 1);
}

void NormalEnemyBehaviour::Update()
{
	gameObject->GetTransform()->position.y = sinf(time + delay * 0.16f) * 100;

	if (gameObject->GetTransform()->position.y < 0)time = 0;
	time += 0.016f;
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,0,1,0},{1,0,0,1} });
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();
	graphicsDevice->GetMeshManager()->GetMesh("LineCube")->Draw();
}

void NormalEnemyBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}
