#include "PlayerBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "Header/Math/Vector3.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Camera2D.h"

PlayerBehaviour::PlayerBehaviour()
	: vel({})
	, isJump(false)
	, input(nullptr)
	, mainCamera(nullptr)
	, fuelValue(1000)
	, MAX_FUEL(1000)
	, CHARGE_FUEL(1)
{
	mainCamera = new PlayerCamera();
}

PlayerBehaviour::~PlayerBehaviour()
{
	delete mainCamera;
}

void PlayerBehaviour::Start()
{
	input = GatesEngine::Input::GetInstance();
	isJump = true;
	gameObject->GetTransform()->position.y = 1000;
	fuelValue = MAX_FUEL;
}

void PlayerBehaviour::Update()
{
	vel.y -= 0.25f;
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::SPACE))
	{
		if (fuelValue > 0)
		{
			vel.y += 1;
			fuelValue -= CHARGE_FUEL;
		}
	}

	const float MAX_VEL_Y = 10;
	if (vel.y >= MAX_VEL_Y)vel.y = MAX_VEL_Y;
	//ˆÚ“®ˆ—
	GatesEngine::Math::Axis cameraAxis = mainCamera->GetRotation().GetAxis();
	GatesEngine::Math::Axis playerAxis = gameObject->GetTransform()->GetMatrix().GetAxis();

	if (!input->GetMouse()->GetCheckHitButton(GatesEngine::MouseButtons::RIGHT_CLICK))
	{
		GatesEngine::Math::Vector3 moveVector = {};
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::W))
		{
			moveVector += playerAxis.z;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::S))
		{
			moveVector -= playerAxis.z;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::D))
		{
			moveVector += playerAxis.x;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::A))
		{
			moveVector -= playerAxis.x;
		}

		const float SPEED = 5;
		gameObject->GetTransform()->position += moveVector.Normalize() * SPEED;
	}

	if (vel.y <= -50)vel.y = -50;
	gameObject->GetTransform()->position += vel;

	GatesEngine::Math::Vector3 a = mainCamera->GetRotation().GetAxis().z;
	gameObject->GetTransform()->rotation.y = atan2f(a.x, a.z);

	GatesEngine::Math::Vector3 pos = gameObject->GetTransform()->position;

	//UŒ‚
	if (input->GetMouse()->GetCheckPressTrigger(GatesEngine::MouseButtons::LEFT_CLICK))
	{
		for (int i = 0; i < (int)bullets.size(); ++i)
		{
			if (!(*bullets[i]).IsUse())
			{
				(*bullets[i]).Shot(cameraAxis.z.Normalize());
				break;
			}
		}
	}
	for (int i = 0; i < (int)bullets.size(); ++i)
	{
		if (!(*bullets[i]).IsUse())
		{
			(*bullets[i]).SetPos(gameObject->GetTransform()->position);
		}
	}

	//ƒJƒƒ‰‚Ìˆ—
	GatesEngine::Math::Vector3 oldCameraPos = mainCamera->GetPosition();
	GatesEngine::Math::Vector3 back = -gameObject->GetTransform()->GetForward();
	mainCamera->SetPosition({ GatesEngine::Math::Vector3(pos.x,pos.y,pos.z) });
	GatesEngine::Math::Vector3 newCameraPos = mainCamera->GetPosition();
}

void PlayerBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();

}

void PlayerBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	float persent = fuelValue / MAX_FUEL;
	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set()
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1,persent * 10,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920,1080,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1,0,0,1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();
}

void PlayerBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}

void PlayerBehaviour::OnCollision(GatesEngine::Collider* hitCollider)
{
	if (hitCollider->GetGameObject()->GetTag() == "block")
	{
		GatesEngine::Math::Vector3 otherOffSet = hitCollider->GetGameObject()->GetTransform()->position;
		GatesEngine::Math::Vector3 otherColliderPos = otherOffSet + hitCollider->GetTransform()->position;
		GatesEngine::Math::Vector3 otherGameObjectSize = hitCollider->GetGameObject()->GetTransform()->scale;
		GatesEngine::Math::Vector3 otherColliderSize = hitCollider->GetTransform()->scale;

		GatesEngine::Math::Vector3 offSet = gameObject->GetTransform()->position;
		GatesEngine::Math::Vector3 colliderPos = otherOffSet + gameObject->GetCollider()->GetTransform()->position;
		GatesEngine::Math::Vector3 gameObjectSize = gameObject->GetTransform()->scale;
		GatesEngine::Math::Vector3 colliderSize = gameObject->GetCollider()->GetTransform()->scale;

		gameObject->GetTransform()->position.y = otherColliderPos.y + (otherGameObjectSize.y * otherColliderSize.y / 2 + gameObjectSize.y * colliderSize.y / 2);
		vel = {};
		isJump = false;
		if (fuelValue >= MAX_FUEL)fuelValue = MAX_FUEL;
		fuelValue += CHARGE_FUEL;
	}
}

void PlayerBehaviour::SetCamera(PlayerCamera* pCamera)
{
	mainCamera = pCamera;
}

PlayerCamera* PlayerBehaviour::GetSetCamera()
{
	return mainCamera;
}

void PlayerBehaviour::AddBullet(PlayerBullet* newBullet)
{
	bullets.push_back(newBullet);
}
