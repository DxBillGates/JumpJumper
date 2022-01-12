#include "PlayerGPUParticleEmitterBehaviour.h"

PlayerGPUParticleEmitterBehaviour::PlayerGPUParticleEmitterBehaviour()
	: DefaultGPUParticleEmitterBehaviour()
{
}

PlayerGPUParticleEmitterBehaviour::~PlayerGPUParticleEmitterBehaviour()
{
}

void PlayerGPUParticleEmitterBehaviour::Start()
{
	DefaultGPUParticleEmitterBehaviour::Start();

	emitterData->MAX_LIFE = 1;
	emitterData->startForce = { 1/2,1/2,1/2 };
	emitterData->startForceMin = { -1,-1,-1 };
}
