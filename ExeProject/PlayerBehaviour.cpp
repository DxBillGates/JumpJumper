#include "PlayerBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"

void PlayerBehaviour::Start()
{
	input = GatesEngine::Input::GetInstance();
	isJump = false;
	combo = 0;
	isAnimation = false;
}

void PlayerBehaviour::Update()
{
	if (!isAnimation)vel -= GatesEngine::Math::Vector3(0, 0.981f / 2, 0);
	else
	{
		if (animationTime >= 1)
		{
			animationTime = 1;
		}

		const float MIN_ANGLE = 0;
		const float MAX_ANGLE = 180;

		using namespace GatesEngine::Math;
		gameObject->GetTransform()->rotation.x = ConvertToRadian(Lerp(MIN_ANGLE, MAX_ANGLE,Easing::EaseInCirc(animationTime)));


		const float ADD_TIME_VALUE = 0.016f * 2;
		animationTime += ADD_TIME_VALUE;
		if (animationTime >= 1)
		{
			gameObject->GetTransform()->rotation.x = 0;
			animationTime = 0;
			isAnimation = false;
			vel = { 0,-40,0 };
		}
	}

	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	{
		if (isJump)
		{
			const int COMBO_EVENT_VALUE = 5;
			if (combo >= COMBO_EVENT_VALUE)
			{
				isAnimation = true;
				vel = {};
			}
		}
	}
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::SPACE))
	{
		if (!isJump)
		{
			vel = GatesEngine::Math::Vector3(0, 20, 0);
			isJump = true;
		}
	}
	//�ړ�����
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

		const float SPEED = 5;
		gameObject->GetTransform()->position += moveVector.Normalize() * SPEED;
	}
	gameObject->GetTransform()->position += vel;

	if (gameObject->GetTransform()->position.y <= 0)
	{
		vel = {};
		isJump = false;
		combo = 0;
		gameObject->GetTransform()->position.y = 0;
	}

	GatesEngine::Math::Vector3 a = mainCamera->GetRotation()->GetAxis().z;
	gameObject->GetTransform()->rotation.y = atan2f(a.x, a.z);

	GatesEngine::Math::Vector3 pos = gameObject->GetTransform()->position;
	mainCamera->SetPosition({ GatesEngine::Math::Vector3{pos.x,pos.y,pos.z} + GatesEngine::Math::Vector3(0,1000,-70) });
	mainCamera->SetYawPitch({ 0,GatesEngine::Math::ConvertToRadian(70) });
}

void PlayerBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	graphicsDevice->GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	int value = (combo >= 10) ? 10 : combo;
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,(float)1080 - 108 * value + 54,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	graphicsDevice->GetMeshManager()->GetMesh("2DPlane")->Draw();

	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, mainCamera->GetData());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_LINELIST);
	graphicsDevice->GetShaderManager()->GetShader("Line")->Set();
	graphicsDevice->GetMeshManager()->GetMesh("LineCube")->Draw();
}

void PlayerBehaviour::OnCollision(GatesEngine::GameObject* other)
{
	if (other->GetTag() == "enemy" && isJump)
	{
		combo++;
		other->SetEnabled(false);
		vel = GatesEngine::Math::Vector3(0, 20, 0);
		gameObject->GetTransform()->position.y += other->GetTransform()->position.y + other->GetCollider()->GetSize().x;
		gameObject->GetTransform()->position += vel;
		killedValue++;
	}
}

void PlayerBehaviour::SetCamera(GatesEngine::Camera* pCamera)
{
	mainCamera = pCamera;
}

GatesEngine::Camera* PlayerBehaviour::GetSetCamera()
{
	return mainCamera;
}

void PlayerBehaviour::Jump(GatesEngine::Math::Vector3 dir, bool flag)
{
	if (flag)vel = dir;
	vel -= GatesEngine::Math::Vector3(0, 0.981f / 2, 0);

	gameObject->GetTransform()->position += vel;
}

bool PlayerBehaviour::GetSceneTransFlag()
{
	if (killedValue >= 4)
	{
		return true;
	}
	return false;
}
