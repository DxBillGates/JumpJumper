#include "DefaultGPUParticleEmitterBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Manager/TextureManager.h"
#include "Header/Util/Random.h"

DefaultGPUParticleEmitterBehaviour::DefaultGPUParticleEmitterBehaviour()
	: gParticleEmitter(new GatesEngine::GPUParticleEmitter())
	, pComputeShader(nullptr)
	, emitterData(nullptr)
	, tex(nullptr)
	, addParticleCount(1)
	, addParticleCyclePerFrame(1)
{
}

DefaultGPUParticleEmitterBehaviour::~DefaultGPUParticleEmitterBehaviour()
{
	delete gParticleEmitter;
}

void DefaultGPUParticleEmitterBehaviour::Start()
{
	emitterData = gParticleEmitter->GetAddData();
	emitterData->MAX_LIFE = 2;
	addParticleCyclePerFrame = 0;
	tex = GatesEngine::ResourceManager::GetTextureManager()->GetTexture("particleTex");
}

void DefaultGPUParticleEmitterBehaviour::Update()
{
	gParticleEmitter->Update(addParticleCount, addParticleCyclePerFrame);
}

void DefaultGPUParticleEmitterBehaviour::OnSecondDraw()
{
	gParticleEmitter->Draw(nullptr, pComputeShader, gameObject->GetTransform()->position, tex);
}

void DefaultGPUParticleEmitterBehaviour::CreateParticleEmitter(GatesEngine::GPUParticleManager* particleManager, int useParticleValue)
{
	gParticleEmitter->Create(particleManager, useParticleValue);
}

void DefaultGPUParticleEmitterBehaviour::SetComputeShader(GatesEngine::ComputePipeline* setShader)
{
	pComputeShader = setShader;
}

void DefaultGPUParticleEmitterBehaviour::SetAddParticleInfo(int pValue, float cycle)
{
	addParticleCount = pValue;
	addParticleCyclePerFrame = cycle;
}
