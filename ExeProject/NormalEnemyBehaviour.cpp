#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"
#include "Header/Collision/CollisionTreeObject.h"
#include "Header/Collision/CollisionTreeCell.h"

NormalEnemyBehaviour::NormalEnemyBehaviour()
	: hp(10)
{
}

NormalEnemyBehaviour::~NormalEnemyBehaviour()
{
}

void NormalEnemyBehaviour::Start()
{
	hp = 10;
}

void NormalEnemyBehaviour::Update()
{
	if (hp <= 0)
	{
		gameObject->SetEnabled(false);
	}
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsDevice->GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	graphicsDevice->GetMeshManager()->GetMesh("testModel")->Draw();
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