#pragma once
#include "Header/Scene/Scene.h"
#include "Stage1.h"
#include "PlayerBehaviour.h"
#include "Header/Graphics/GPUParticle/GPUParticleManager.h"
#include "Header/Graphics/GPUParticle/GPUParticleEmitter.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"

class Stage1Scene : public GatesEngine::Scene
{
private:
	Stage1 stage;
	PlayerBehaviour* playerBehaviour;
	GatesEngine::GameObject* boss;

	GatesEngine::ComputePipeline* testCS;
	GatesEngine::GPUParticleManager* gpuParticleManager;
	GatesEngine::GPUParticleEmitter gpuParticleEmitter;

	GatesEngine::RenderTexture shadowRenderTex;
	GatesEngine::DepthStencilTexture shadowDepthTex;

	GatesEngine::RenderTexture resultRenderTex;
	GatesEngine::DepthStencilTexture resultDepthTex;

	GatesEngine::RenderTexture lateDrawResultRenderTex;
	GatesEngine::DepthStencilTexture lateDrawResultDepthTex;

	GatesEngine::RenderTexture resultRenderShadowTex;

	GatesEngine::RenderTexture parlinNoiseTex;
	GatesEngine::RenderTexture parlinNoiseHeightMapTex;
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

