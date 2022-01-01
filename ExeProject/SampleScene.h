#pragma once
#include "Header/Scene/Scene.h"
#include "StageManager.h"
#include "Stage1.h"
#include "Header/Math/Math.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"
#include "Header/Graphics/ComputePipeline.h"
#include "TutorialSystem.h"

class SampleScene : public GatesEngine::Scene
{
private:
private:
	Stage1 stage;
	PlayerBehaviour* playerBehaviour;

	GatesEngine::ComputePipeline* testCS;

	GatesEngine::GPUParticleManager* gpuParticleManager;

	GatesEngine::RenderTexture shadowRenderTex;
	GatesEngine::DepthStencilTexture shadowDepthTex;

	GatesEngine::RenderTexture resultRenderTex;
	GatesEngine::DepthStencilTexture resultDepthTex;

	GatesEngine::RenderTexture lateDrawResultRenderTex;
	GatesEngine::DepthStencilTexture lateDrawResultDepthTex;

	GatesEngine::RenderTexture resultRenderShadowTex;

	GatesEngine::RenderTexture parlinNoiseTex;
	GatesEngine::RenderTexture parlinNoiseHeightMapTex;

	GatesEngine::RenderTexture redrawRenderTexture;
	GatesEngine::DepthStencilTexture redrawDepthTex;
	GatesEngine::RenderTexture subPostprocessTexture;
	GatesEngine::RenderTexture brightnessTexture;
	GatesEngine::RenderTexture blurRenderTexture;

	GatesEngine::RenderTexture blurRenderTexture2;
	GatesEngine::RenderTexture blurRenderTexture3;

	GatesEngine::RenderTexture blurPlusParticleTex;

	GatesEngine::Math::GaussFilterData gaussData[6];

	GatesEngine::RenderTexture blurRenderTextures[6];
	TutorialSystem* tutorialSystem;

	bool isIncreaseBlack;
	bool isDecreaseBlack;
	bool isSceneTransition;
	float black;
	GatesEngine::RenderTexture sceneRenderTexture;
	GatesEngine::DepthStencilTexture sceneDepthTexture;
public:
	SampleScene();
	SampleScene(const char* sceneName);
	SampleScene(const char* sceneName, GatesEngine::Application* app);
	~SampleScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LateDraw() override;
};
