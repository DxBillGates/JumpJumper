#include "GPUParticleEmitterBehaviour.h"
#include "Header/GameObject/GameObject.h"

GPUParticleEmitterBehaviour::GPUParticleEmitterBehaviour()
	: gParticleEmitter(new GatesEngine::GPUParticleEmitter())
	, pComputeShader(nullptr)
	, pInitializeComputeShader(nullptr)
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
}

void GPUParticleEmitterBehaviour::ExternalDraw()
{
	if (!pComputeShader)return;
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

void GPUParticleEmitterBehaviour::SetInitializeShader(GatesEngine::ComputePipeline* setShader)
{
	pInitializeComputeShader = setShader;
}

void GPUParticleEmitterBehaviour::DispatchInitializeShader()
{
	if (!pInitializeComputeShader)return;
	gParticleEmitter->Draw(nullptr, pInitializeComputeShader, gameObject->GetTransform()->position);
}
