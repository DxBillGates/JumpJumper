#pragma once
#include "DefaultGPUParticleEmitterBehaviour.h"

class PlayerGPUParticleEmitterBehaviour : public DefaultGPUParticleEmitterBehaviour
{
private:
public:
	PlayerGPUParticleEmitterBehaviour();
	~PlayerGPUParticleEmitterBehaviour();
	void Start() override;
};