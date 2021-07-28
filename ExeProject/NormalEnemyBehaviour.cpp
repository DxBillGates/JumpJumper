#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"

void NormalEnemyBehaviour::Start()
{
	time = 0;
	delay = (float)(rand() % 10 + 1);
	gameObject->GetTransform()->scale = gameObject->GetCollider()->GetSize();
	hp = 10;
}

void NormalEnemyBehaviour::Update()
{
	if (hp <= 0)
	{
		gameObject->SetEnabled(false);
	}
	gameObject->GetTransform()->position.y = sinf(time + delay * 0.16f) * 100 + y;

	GatesEngine::Transform* transform = gameObject->GetTransform();
	transform->position.x = r * sinf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);
	transform->position.z = r * cosf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);

	//if (gameObject->GetTransform()->position.y < 0)time = 0;
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

	GatesEngine::Math::Matrix4x4 lineCubeMatrix = GatesEngine::Math::Matrix4x4::Scale(gameObject->GetCollider()->GetSize());
	lineCubeMatrix *= GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position);
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, lineCubeMatrix);
	graphicsDevice->GetMeshManager()->GetMesh("LineCube")->Draw();
}

void NormalEnemyBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}

void NormalEnemyBehaviour::Damage(float value)
{
	hp -= value;
}

float NormalEnemyBehaviour::GetHP()
{
	return hp;
}

void NormalEnemyBehaviour::SetR(float value)
{
	r = value;
}

void NormalEnemyBehaviour::SetY(float value)
{
	y = value;
}
