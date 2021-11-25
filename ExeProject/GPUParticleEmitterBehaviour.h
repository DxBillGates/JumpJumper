#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/GPUParticle/GPUParticleEmitter.h"
class GPUParticleEmitterBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::GPUParticleEmitter* gParticleEmitter;
	GatesEngine::ComputePipeline* pComputeShader;
public:
	GPUParticleEmitterBehaviour();
	~GPUParticleEmitterBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void CreateParticleEmitter(GatesEngine::GPUParticleManager* particleManager, int useParticleValue);
	void SetComputeShader(GatesEngine::ComputePipeline* setShader);
};

