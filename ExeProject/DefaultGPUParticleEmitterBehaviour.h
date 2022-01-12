#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/GPUParticle/GPUParticleEmitter.h"
#include "Header/Graphics/Texture.h"

class DefaultGPUParticleEmitterBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::GPUParticleEmitter* gParticleEmitter;
	GatesEngine::ComputePipeline* pComputeShader;
protected:
	GatesEngine::EmitterData* emitterData;
	GatesEngine::Texture* tex;
	int addParticleCount;
	float addParticleCyclePerFrame;
public:
	DefaultGPUParticleEmitterBehaviour();
	virtual ~DefaultGPUParticleEmitterBehaviour();
	void Start() override;
	void Update() override;
	void OnSecondDraw() override;
	void CreateParticleEmitter(GatesEngine::GPUParticleManager* particleManager, int useParticleValue);
	void SetComputeShader(GatesEngine::ComputePipeline* setShader);
	void SetAddParticleInfo(int pValue, float cycle);
};
