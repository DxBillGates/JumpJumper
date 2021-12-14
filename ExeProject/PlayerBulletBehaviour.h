#pragma once
#include "GPUParticleEmitterBehaviour.h"
#include "Header/Component/Behaviour.h"
#include "PlayerBullet.h"

class BulletBehaviour : public GatesEngine::Behaviour,public Bullet
{
private:
	GPUParticleEmitterBehaviour* gpuParticleEmitter;
	GatesEngine::Math::Vector3 targetPos;
public:
	BulletBehaviour();
	~BulletBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnSecondDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::Collider* otherCollider) override;
	void SetGPUParticleEmitter(GPUParticleEmitterBehaviour* behaviour);
};