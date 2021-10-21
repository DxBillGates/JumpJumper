#include "TrankingEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Component/Collider.h"


void TrankingEnemyBehaviour::Start()
{
}

void TrankingEnemyBehaviour::Update()
{
	if (!trakingGameObject)return;
	GatesEngine::Transform* transform = gameObject->GetTransform();
	GatesEngine::Transform* targetTransform = trakingGameObject->GetTransform();
	GatesEngine::Math::Vector3 vec = GatesEngine::Math::Vector3::Normalize(targetTransform->position - transform->position);
	transform->position += vec;
}

void TrankingEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();
}

void TrankingEnemyBehaviour::OnCollision(GatesEngine::GameObject* other)
{
	if (other->GetName() == "player")
	{
		GatesEngine::Transform* transform = gameObject->GetTransform();
		GatesEngine::Transform* targetTransform = trakingGameObject->GetTransform();
		GatesEngine::Math::Vector3 vec = GatesEngine::Math::Vector3::Normalize(targetTransform->position - transform->position);
		transform->position -= vec * 100;
	}
}

void TrankingEnemyBehaviour::SetTrakingGameObject(GatesEngine::GameObject* setObject)
{
	trakingGameObject = setObject;
}
