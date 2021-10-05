#include "PlayerBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "Header/Math/Vector3.h"

PlayerBehaviour::PlayerBehaviour()
	: vel({})
	, isJump(false)
	//, combo(0)
	, input(nullptr)
	, mainCamera(nullptr)
	//, isAnimation(false)
	//, animationTime(0)
	//, killedValue(0)
	, fuelValue(100)
{
}

PlayerBehaviour::~PlayerBehaviour()
{
}

void PlayerBehaviour::Start()
{
	input = GatesEngine::Input::GetInstance();
	isJump = true;
	//combo = 0;
	//isAnimation = false;
	gameObject->GetTransform()->position.y = 1000;
	gameObject->GetTransform()->scale = gameObject->GetCollider()->GetSize();
	fuelValue = 100;
}

void PlayerBehaviour::Update()
{
	vel.y -= 0.25f;
	//if (!isAnimation)vel -= GatesEngine::Math::Vector3(0, 0.981f / 5, 0);
	//else
	//{
	//	if (animationTime >= 1)
	//	{
	//		animationTime = 1;
	//	}

	//	const float MIN_ANGLE = 0;
	//	const float MAX_ANGLE = 180;

	//	using namespace GatesEngine::Math;
	//	gameObject->GetTransform()->rotation.x = ConvertToRadian(Lerp(MIN_ANGLE, MAX_ANGLE, Easing::EaseInCirc(animationTime)));


	//	const float ADD_TIME_VALUE = 0.016f * 2;
	//	animationTime += ADD_TIME_VALUE;
	//	if (animationTime >= 1)
	//	{
	//		gameObject->GetTransform()->rotation.x = 0;
	//		animationTime = 0;
	//		isAnimation = false;
	//		vel = { 0,-40,0 };
	//	}
	//}

	//if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	//{
	//	if (isJump)
	//	{
	//		const int COMBO_EVENT_VALUE = 5;
	//		if (combo >= COMBO_EVENT_VALUE)
	//		{
	//			isAnimation = true;
	//			vel = {};
	//		}
	//	}
	//}
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::SPACE))
	{
		if (fuelValue > 0)
		{
			//if (!isJump)
			//{
			//	vel = GatesEngine::Math::Vector3(0, 20, 0);
			//	isJump = true;
			//}
			//else
			//{
			vel.y += 1;
			fuelValue -= 0.5f;
			//}
		}
	}

	const float MAX_VEL_Y = 10;
	if (vel.y >= MAX_VEL_Y)vel.y = MAX_VEL_Y;
	//ˆÚ“®ˆ—
	//if (input->GetMouse()->GetCheckHitButton(GatesEngine::MouseButtons::RIGHT_CLICK))
	//{
		GatesEngine::Math::Axis cameraAxis = mainCamera->GetRotation()->GetAxis();
		//cameraAxis.z.y = 0;
		GatesEngine::Math::Axis playerAxis = gameObject->GetTransform()->GetMatrix().GetAxis();

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
	//}
	if (vel.y <= -50)vel.y = -50;
	gameObject->GetTransform()->position += vel;

	//if (gameObject->GetTransform()->position.y <= -1000)
	//{
	//	vel = {};
	//	isJump = false;
	//	combo = 0;
	//	gameObject->GetTransform()->position.y = 0;
	//	Start();
	//}

	GatesEngine::Math::Vector3 a = mainCamera->GetRotation()->GetAxis().z;
	gameObject->GetTransform()->rotation.y = atan2f(a.x, a.z);

	GatesEngine::Math::Vector3 pos = gameObject->GetTransform()->position;
	if (pos.y <= 100)
	{
		isJump = false;
		gameObject->GetTransform()->position.y = 100;
		pos = gameObject->GetTransform()->position;
		fuelValue = 100;
	}

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
	mainCamera->SetPosition({ GatesEngine::Math::Vector3(pos.x,pos.y,pos.z)/* + GatesEngine::Math::Vector3(0, 1000, -1000)*/ });
	GatesEngine::Math::Vector3 newCameraPos = mainCamera->GetPosition();
	mainCamera->Update();

	//mainCamera->SetPosition(GatesEngine::Math::Vector3::Lerp(oldCameraPos, newCameraPos, 0.025f));
	//mainCamera->SetYawPitch({ 0,GatesEngine::Math::ConvertToRadian(45) });
}

void PlayerBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//graphicsDevice->GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	////int value = (combo >= 10) ? 10 : combo;
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0,(float)1080 - 108 + 54,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//graphicsDevice->GetMeshManager()->GetMesh("2DPlane")->Draw();

	graphicsDevice->GetShaderManager()->GetShader("MeshShadowShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, mainCamera->GetData());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();

}

void PlayerBehaviour::OnCollision(GatesEngine::GameObject* other)
{
	if (other->GetTag() == "enemy" && isJump)
	{
		NormalEnemyBehaviour* enemy = other->GetComponent<NormalEnemyBehaviour>();
		enemy->Damage();
		//combo++;
		//other->SetEnabled(false);
		vel = GatesEngine::Math::Vector3(0, 20, 0);
		//vel.y += (combo * combo) / 10;
		gameObject->GetTransform()->position.y = other->GetTransform()->position.y + other->GetCollider()->GetSize().x;
		gameObject->GetTransform()->position += vel;
		//if (enemy->GetHP() <= 0)
		//{
		//	killedValue++;
		//}
	}
	if (other->GetTag() == "block")
	{
		gameObject->GetTransform()->position.y = other->GetTransform()->position.y + (other->GetCollider()->GetSize().y / 2 + gameObject->GetCollider()->GetSize().y / 2);
		vel = {};
		isJump = false;
		fuelValue += 10;
		if (fuelValue >= 100)fuelValue = 100;
	}
	if (other->GetTag() == "goal")
	{
		Start();
	}
	if (other->GetTag() == "trak_enemy")
	{
		GatesEngine::Math::Vector3 vec = GatesEngine::Math::Vector3::Normalize(other->GetTransform()->position - gameObject->GetTransform()->position);
		vel -= vec * 10;
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

bool PlayerBehaviour::GetSceneTransFlag()
{
	//if (killedValue >= 4)
	//{
	//	return true;
	//}
	return false;
}

void PlayerBehaviour::ResetKillCount()
{
	//killedValue = 0;
}

int PlayerBehaviour::GetKillCount()
{
	//return killedValue;
	return 0;
}

void PlayerBehaviour::AddBullet(PlayerBullet* newBullet)
{
	bullets.push_back(newBullet);
}
