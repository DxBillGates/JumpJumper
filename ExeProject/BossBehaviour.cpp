#include "BossBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"
#include "Header/Graphics/Camera3D.h"

void BossBehaviour::InitState()
{
	state = BossState::NONE;
	isJoining = false;
	joiningTime = 1;
	isLefting = false;
	leftingTime = 1;
}

void BossBehaviour::JoinOrLeft(BossState state)
{
	GatesEngine::Math::Vector3 endPos;
	float easingValue = 0;

	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_TIME = 3;


	switch (state)
	{
	case BossState::JOIN:
		endPos = {};
		easingValue = GatesEngine::Math::Easing::EaseInExpo(joiningTime);
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		joiningTime += PER_FRAME / MAX_TIME;
		break;
	case BossState::LEFT:
		endPos = preLerpPos;
		endPos.y = preLerpPos.y + 10000;
		easingValue = GatesEngine::Math::Easing::EaseOutExpo(leftingTime);
		gameObject->GetTransform()->position = GatesEngine::Math::Vector3::Lerp(preLerpPos, endPos, easingValue);
		leftingTime += PER_FRAME / MAX_TIME;
		break;
	default:
		break;
	}

	if (joiningTime > 1 || leftingTime > 1)
	{
		InitState();
		stopFlag = true;
	}


}

void BossBehaviour::Stoping()
{
	state = BossState::HEALING;
	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_STOPING_TIME = 5;

	const int HEALING_VALUE_PER_FRAME = 1;

	hp = GatesEngine::Math::Lerp(0, MAX_HP, GatesEngine::Math::Easing::EaseInOutQuint(stopingTime));

	if (stopingTime >= 1)
	{
		stopFlag = false;
		stopingTime = 0;
		hp = MAX_HP;
	}

	stopingTime += PER_FRAME / MAX_STOPING_TIME;
}

BossBehaviour::BossBehaviour()
	: state(BossState::NONE)
	, isJoining(false)
	, joiningTime(0)
	, isLefting(false)
	, leftingTime(0)
	, stopFlag(false)
	, stopingTime(0)
	, MAX_HP(100)
	, hp(MAX_HP)
{
}

BossBehaviour::~BossBehaviour()
{
}

void BossBehaviour::Start()
{
	InitState();
	stopFlag = false;
	stopingTime = 0;
	hp = 0;
}

void BossBehaviour::Update()
{
	if (isJoining)JoinOrLeft(BossState::JOIN);
	else if (isLefting)JoinOrLeft(BossState::LEFT);

	if (state == BossState::NONE && stopFlag)
	{
		GatesEngine::Camera3D* mainCamera = dynamic_cast<GatesEngine::Camera3D*>(gameObject->GetGraphicsDevice()->GetMainCamera());

		if (mainCamera)
		{
			mainCamera->SetClip();
		}
	}

	if (stopFlag)Stoping();
}

void BossBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("testModel")->Draw();


	GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(500);
	GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(1000, 0, 0));

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();


	posMatrix = GatesEngine::Math::Matrix4x4::Translate(gameObject->GetTransform()->position + GatesEngine::Math::Vector3(-1000, 0, 0));

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * posMatrix);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();
}

void BossBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	float persent = hp / MAX_HP;
	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 1920 * persent,50,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920 / 2,25,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();
}

void BossBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
}

void BossBehaviour::SetBossState(BossState state)
{
	InitState();
	this->state = state;
	preLerpPos = gameObject->GetTransform()->position;

	switch (state)
	{
	case BossState::NONE:
		isJoining = false;
		joiningTime = 0;
		isLefting = false;
		leftingTime = 0;
		break;
	case BossState::JOIN:
		isJoining = true;
		joiningTime = 0;
		break;
	case BossState::LEFT:
		isLefting = true;
		leftingTime = 0;
		break;
	}
}
