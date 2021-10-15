#pragma once
#include "Header/Scene/Scene.h"
#include "Stage1.h"
#include "SceneTranslater.h"
#include "PlayerBehaviour.h"
#include "Header/Graphics/GPUParticle/GPUParticleManager.h"
#include "Header/Graphics/GPUParticle/GPUParticleEmitter.h"

class Stage1Scene : public GatesEngine::Scene,public Stage1
{
private:
	Stage1 stage;
	SceneTranslater sceneTranslater;
	PlayerBehaviour* playerBehaviour;

	GatesEngine::ComputePipeline* testCS;
	GatesEngine::GPUParticleManager* gpuParticleManager;
	GatesEngine::GPUParticleEmitter gpuParticleEmitter;
public:
	Stage1Scene();
	Stage1Scene(const char* sceneName);
	Stage1Scene(const char* sceneName, GatesEngine::Application* app);
	~Stage1Scene();
	void Initialize();
	void Update();
	void Draw();
	void LateDraw() override;
};

