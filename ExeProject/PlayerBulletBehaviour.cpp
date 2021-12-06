#include "PlayerBulletBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

PlayerBulletBehaviour::PlayerBulletBehaviour()
	: gpuParticleEmitter(nullptr)
{
}

PlayerBulletBehaviour::~PlayerBulletBehaviour()
{
}

void PlayerBulletBehaviour::Start()
{
	//gameObject->GetTransform()->scale = gameObject->GetCollider()->GetSize() - 5;
	Initialize();
	setPos = { 0,-1000,0 };
}

void PlayerBulletBehaviour::Update()
{
	if (isUse)
	{
		if (!isHoming)
		{
			vel += shotVector;
			gameObject->GetTransform()->position += vel;
		}
		else
		{
			if (!target)return;
			GatesEngine::Math::Vector3 diff = target->GetTransform()->position - gameObject->GetTransform()->position;
			GatesEngine::Math::Vector3 acc;
			acc += (diff - vel * homingTime) * 2.0f / (homingTime * homingTime);

			homingTime -= 0.016f / 2.0f;

			//if (GatesEngine::Math::Vector3::Distance(target->GetTransform()->position, gameObject->GetTransform()->position) < 500)
			//{
			//	isHoming = false;
			//}
			//if (acc.Length() > 10000)acc = acc.Normalize() * 10000;
			//float MAX_HOMING_TIME = 0.2f;
			//if (homingTime < MAX_HOMING_TIME)homingTime = -MAX_HOMING_TIME;

			vel += acc * 0.016f / 2.0f;

			if (homingTime < maxHomingTime)
			{
				vel = vel.Normalize() * 4000;
			}
			gameObject->GetTransform()->position += vel * 0.016f / 2.0f;
		}
	}
	else
	{
		gameObject->GetTransform()->position = setPos;
	}
	PlayerBullet::Update();
}

void PlayerBulletBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Sphere")->Draw();
}

void PlayerBulletBehaviour::OnLateDraw()
{
	if (!isUse)return;
	if (!gpuParticleEmitter)return;
	gpuParticleEmitter->ExternalDraw();
}

void PlayerBulletBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == "enemy")
	{
		isHoming = false;
		vel = {};
		gameObject->GetTransform()->position = { 0,-1000,0 };
		//Initialize();

	}
	//if (otherCollider->GetGameObject()->GetTag() == "block")
	//{
	//	isUse = false;
	//	Initialize();
	//}
}

void PlayerBulletBehaviour::SetGPUParticleEmitter(GatesEngine::Behaviour* behaviour)
{
	gpuParticleEmitter = behaviour;
}
