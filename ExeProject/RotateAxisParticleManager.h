#pragma once
#include "RotateAxisBehaviour.h"
#include "DefaultGPUParticleEmitterBehaviour.h"
#include <vector>

class RotateAxisParticleManager
{
private:
	std::vector<RotateAxisBehaviour*> axisBehaviours;
	std::vector<DefaultGPUParticleEmitterBehaviour*> particleBehaviours;
public:
	RotateAxisParticleManager();
	~RotateAxisParticleManager();
	void Initialize();
	void Add(RotateAxisBehaviour* axis,DefaultGPUParticleEmitterBehaviour* particle);
	void StartParticles(const GatesEngine::Math::Vector3& center);
	void ReverseParticle();
	bool GetIsEndParticles(const GatesEngine::Math::Vector3& center);
	void EndParticles();
};