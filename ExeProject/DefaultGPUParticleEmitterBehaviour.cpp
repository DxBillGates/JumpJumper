#include "DefaultGPUParticleEmitterBehaviour.h"
#include "Header/GameObject/GameObject.h"

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
	emitterData->MAX_LIFE = 4;
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
