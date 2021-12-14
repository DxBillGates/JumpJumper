#include "PlayerBulletBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

BulletBehaviour::BulletBehaviour()
	: gpuParticleEmitter(nullptr)
{
}

BulletBehaviour::~BulletBehaviour()
{
}

void BulletBehaviour::Start()
{
	//gameObject->GetTransform()->scale = gameObject->GetCollider()->GetSize() - 5;
	Initialize();
	setPos = { 0,-1000,0 };
	targetPos = {};
}

void BulletBehaviour::Update()
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

			const float PER_FRAME = 1.0f / 60.0f;
			const float SPEED = 0.5f;
			homingTime -= PER_FRAME * SPEED;

			const float MAX_ACC = 100000;
			if (acc.Length() > MAX_ACC)
			{
				acc = acc.Normalize() * MAX_ACC;
			}
			vel += acc * PER_FRAME * SPEED;

			gameObject->GetTransform()->position += vel * PER_FRAME * SPEED;
		}
	}
	else
	{
		gameObject->GetTransform()->position = setPos;
	}
	Bullet::Update();
}

void BulletBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Sphere")->Draw();
}

void BulletBehaviour::OnSecondDraw()
{
	if (!isUse)
	{
		gpuParticleEmitter->DispatchInitializeShader();
		return;
	}
	if (!gpuParticleEmitter)return;
	gpuParticleEmitter->ExternalDraw();
}

void BulletBehaviour::OnLateDraw()
{
	//if (!isUse)
	//{
	//	gpuParticleEmitter->DispatchInitializeShader();
	//	return;
	//}
	//if (!gpuParticleEmitter)return;
	//gpuParticleEmitter->ExternalDraw();
}

void BulletBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	if (otherCollider->GetGameObject()->GetTag() == "enemy" || otherCollider->GetGameObject()->GetTag() == "player")
	{
		isHoming = false;
		vel = {};
		gameObject->GetTransform()->position = { 0,-1000,0 };
	}

	if (gameObject->GetTag() == "playerBullet" && otherCollider->GetGameObject()->GetTag() == "Boss")
	{
		isHoming = false;
		vel = {};
		gameObject->GetTransform()->position = { 0,-1000,0 };
	}
}

void BulletBehaviour::SetGPUParticleEmitter(GPUParticleEmitterBehaviour* behaviour)
{
	gpuParticleEmitter = behaviour;
}
