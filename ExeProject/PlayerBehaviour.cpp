#include "PlayerBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"

void PlayerBehaviour::Start()
{
	input = GatesEngine::Input::GetInstance();
}

void PlayerBehaviour::Update()
{
	vel -= GatesEngine::Math::Vector3(0,0.981f/2,0);
	if(input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	{
		vel = GatesEngine::Math::Vector3(0,20,0);
	}

	if (!input->GetMouse()->GetCheckHitButton(GatesEngine::MouseButtons::RIGHT_CLICK))
	{
		GatesEngine::Math::Axis cameraAxis = mainCamera->GetRotation()->GetAxis();
		cameraAxis.z.y = 0;

		GatesEngine::Math::Vector3 moveVector = {};
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::W))
		{
			moveVector += cameraAxis.z;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::S))
		{
			moveVector -= cameraAxis.z;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::D))
		{
			moveVector += cameraAxis.x;
		}
		if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::A))
		{
			moveVector -= cameraAxis.x;
		}

		//スピード値
		const float SPEED = 10;
		gameObject->GetTransform()->position += moveVector.Normalize() * SPEED;
	}
	gameObject->GetTransform()->position += vel;

	if (gameObject->GetTransform()->position.y <= 0)
	{
		vel = {};
		gameObject->GetTransform()->position.y = 0;
	}

	GatesEngine::Math::Vector3 a = mainCamera->GetRotation()->GetAxis().z;
	gameObject->GetTransform()->rotation.y = atan2f(a.x, a.z);
}

void PlayerBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0,gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();

}

void PlayerBehaviour::OnCollision(GatesEngine::GameObject* other)
{
	printf("!!\n");
	vel = GatesEngine::Math::Vector3(0, 20, 0);
}

void PlayerBehaviour::SetCamera(GatesEngine::Camera* pCamera)
{
	mainCamera = pCamera;
}
