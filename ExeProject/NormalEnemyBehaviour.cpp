#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"
#include "Header/Collision/CollisionTreeObject.h"
#include "Header/Collision/CollisionTreeCell.h"

NormalEnemyBehaviour::NormalEnemyBehaviour()
	: time(0)
	, delay(0)
	, hp(10)
	, r(0)
	, y(0)
	, moveVector({})
	, targetPos({})
{
}

NormalEnemyBehaviour::~NormalEnemyBehaviour()
{
}

void NormalEnemyBehaviour::Start()
{
	time = 0;
	delay = (float)(rand() % 10 + 1);
	hp = 10;
}

void NormalEnemyBehaviour::Update()
{
	if (hp <= 0)
	{
		gameObject->SetEnabled(false);
	}
	//gameObject->GetTransform()->position.y = sinf(time + delay * 0.16f) * 100 + y;

	GatesEngine::Transform* transform = gameObject->GetTransform();
	//transform->position.x = r * sinf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);
	//transform->position.z = r * cosf((2 * GatesEngine::Math::PI / 360) * time*10 * delay / 10);

	//if (gameObject->GetTransform()->position.y < 0)time = 0;
	time += 0.016f;


	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::H))
	{
		GatesEngine::Collider* collider = gameObject->GetCollider();

		if (collider)
		{
			GatesEngine::CollisionTreeObject* collisionObj = collider->GetCollisionTreeObject();
			GatesEngine::CollisionTreeCell* collisionCell = (collisionObj) ? collisionObj->GetSettingCell() : nullptr;
			GatesEngine::CollisionTreeObject* collisionObjArray = (collisionCell) ? collisionCell->GetFirstObject() : nullptr;

			std::vector<GatesEngine::Math::Vector3> betweenVectors;

			//八分木で最短オブジェクトを捜査
			while (collisionObjArray)
			{
				if (collisionObjArray->GetCollider()->GetGameObject()->GetTag() == "block")
				{
					betweenVectors.push_back(collisionObjArray->GetCollider()->GetGameObject()->GetTransform()->position - gameObject->GetTransform()->position);
				}
				collisionObjArray = collisionObjArray->GetNextObject();
			}

			float minLength = 100000;
			int i = 0;
			int target = 0;
			for (auto vector : betweenVectors)
			{
				float distance = vector.Length();
				if (distance < minLength && distance >= 1.f)
				{
					minLength = distance;
					target = i;
				}
				++i;
			}
			if (!betweenVectors.empty())
			{
				if (minLength != 0)
				{
					targetPos = betweenVectors[target] + transform->position;
					moveVector = betweenVectors[target].Normalize();
				}
			}
		}

	}

	float targetDistance = GatesEngine::Math::Vector3::Distance(targetPos, transform->position);
	//printf("%f\n", targetDistance);
	if (targetDistance <= 0.5f)moveVector = {};
	gameObject->GetTransform()->position += moveVector;
}

	void NormalEnemyBehaviour::OnDraw()
	{
		GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

		graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphicsDevice->GetShaderManager()->GetShader("MeshShadowShader")->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0,0,1} });
		graphicsDevice->GetMeshManager()->GetMesh("testModel")->Draw();
	}

	void NormalEnemyBehaviour::OnCollision(GatesEngine::GameObject * other)
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
