#pragma once
#include "DefaultGPUParticleEmitterBehaviour.h"

class ExplosionGPUParticleEmitterBehaviour : public DefaultGPUParticleEmitterBehaviour
{
private:
public:
	ExplosionGPUParticleEmitterBehaviour();
	~ExplosionGPUParticleEmitterBehaviour();
	void Start() override;
};