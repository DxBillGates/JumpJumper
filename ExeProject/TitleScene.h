#pragma once
#include "Header/Scene/Scene.h"
#include "Header/Graphics/Camera3D.h"
#include "Header/Input/Input.h"
#include "TitleManager.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"
#include "Header/Graphics/ComputePipeline.h"

class TitleScene : public GatesEngine::Scene
{
private:
	using Vector3 = GatesEngine::Math::Vector3;
	using Vector4 = GatesEngine::Math::Vector4;
	using Matrix4x4 = GatesEngine::Math::Matrix4x4;
private:
	GatesEngine::Camera3D* mainCamera;
	GatesEngine::Input* input;
	TitleManager titleManager;

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

	float black;
	bool isIncreaseBlack;
	bool isDecreaseBlack;
	bool isSceneTransition;
	ButtonUIType sceneTransType;

	GatesEngine::RenderTexture sceneRenderTexture;
	GatesEngine::DepthStencilTexture sceneDepthTexture;

	GatesEngine::ComputePipeline* testCS;
	GatesEngine::GPUParticleManager* gpuParticleManager;
public:
	TitleScene();
	TitleScene(const char* sceneName);
	TitleScene(const char* sceneName,GatesEngine::Application* app);
	~TitleScene();
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LateDraw() override;
};
