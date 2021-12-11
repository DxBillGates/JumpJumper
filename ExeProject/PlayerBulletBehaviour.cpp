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
	targetPos = {};
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

			targetPos = target->GetTransform()->position;
			GatesEngine::Math::Vector3 diff = targetPos - gameObject->GetTransform()->position;
			GatesEngine::Math::Vector3 acc;
			acc += (diff - vel * homingTime) * 2.0f / (homingTime * homingTime);

			homingTime -= 0.016f / 2.0f;

			const float MAX_ACC = 100000;
			if (acc.Length() > MAX_ACC)
			{
				acc = acc.Normalize() * MAX_ACC;
			}
			vel += acc * 0.016f / 2.0f;

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

void PlayerBulletBehaviour::OnSecondDraw()
{
	if (!isUse)
	{
		gpuParticleEmitter->DispatchInitializeShader();
		return;
	}
	if (!gpuParticleEmitter)return;
	gpuParticleEmitter->ExternalDraw();
}

void PlayerBulletBehaviour::OnLateDraw()
{
	if (!isUse)
	{
		gpuParticleEmitter->DispatchInitializeShader();
		return;
	}
	if (!gpuParticleEmitter)return;
	gpuParticleEmitter->ExternalDraw();
}

void PlayerBulletBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == "enemy" || otherCollider->GetGameObject()->GetTag() == "player")
	{
		isHoming = false;
		vel = {};
		gameObject->GetTransform()->position = { 0,-1000,0 };
	}
}

void PlayerBulletBehaviour::SetGPUParticleEmitter(GPUParticleEmitterBehaviour* behaviour)
{
	gpuParticleEmitter = behaviour;
}
