#include "ExplosionGPUParticleEmitterBehaviour.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Manager/TextureManager.h"

ExplosionGPUParticleEmitterBehaviour::ExplosionGPUParticleEmitterBehaviour()
	: DefaultGPUParticleEmitterBehaviour()
{
}

ExplosionGPUParticleEmitterBehaviour::~ExplosionGPUParticleEmitterBehaviour()
{
}

void ExplosionGPUParticleEmitterBehaviour::Start()
{
	DefaultGPUParticleEmitterBehaviour::Start();
	emitterData->MAX_LIFE = 5;
	emitterData->startForce = 5;
	emitterData->vel = { 0,-5,0 };
	addParticleCount = 60;
	addParticleCyclePerFrame = 60;
	tex = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("fire_particleTex");
}
