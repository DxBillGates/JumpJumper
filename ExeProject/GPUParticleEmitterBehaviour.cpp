#include "GPUParticleEmitterBehaviour.h"
#include "Header/GameObject/GameObject.h"

GPUParticleEmitterBehaviour::GPUParticleEmitterBehaviour()
	: gParticleEmitter(new GatesEngine::GPUParticleEmitter())
	, pComputeShader(nullptr)
{
}

GPUParticleEmitterBehaviour::~GPUParticleEmitterBehaviour()
{
	delete gParticleEmitter;
}

void GPUParticleEmitterBehaviour::Start()
{
}

void GPUParticleEmitterBehaviour::Update()
{
	gParticleEmitter->Update();
}

void GPUParticleEmitterBehaviour::OnDraw()
{
}

void GPUParticleEmitterBehaviour::OnLateDraw()
{
	gParticleEmitter->Draw(nullptr, pComputeShader, gameObject->GetTransform()->position);
}

void GPUParticleEmitterBehaviour::CreateParticleEmitter(GatesEngine::GPUParticleManager* particleManager, int useParticleValue)
{
	gParticleEmitter->Create(particleManager, useParticleValue);
}

void GPUParticleEmitterBehaviour::SetComputeShader(GatesEngine::ComputePipeline* setShader)
{
	pComputeShader = setShader;
}
