#pragma once
#include "Header/Scene/Scene.h"
#include "Stage1.h"
#include "PlayerBehaviour.h"
#include "EnemyManager.h"
#include "Header/Graphics/GPUParticle/GPUParticleManager.h"
#include "Header/Graphics/GPUParticle/GPUParticleEmitter.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"
#include "GameState.h"
#include "BossBehaviour.h"

class Stage1Scene : public GatesEngine::Scene
{
private:
	Stage1 stage;
	PlayerBehaviour* playerBehaviour;
	GatesEngine::GameObject* boss;

	GatesEngine::ComputePipeline* testCS;
	GatesEngine::ComputePipeline* testInitializeCS;
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

	GatesEngine::RenderTexture redrawRenderTexture;
	GatesEngine::DepthStencilTexture redrawDepthTex;
	GatesEngine::RenderTexture subPostprocessTexture;
	GatesEngine::RenderTexture brightnessTexture;
	GatesEngine::RenderTexture blurRenderTexture;

	GatesEngine::RenderTexture blurRenderTexture2;
	GatesEngine::RenderTexture blurRenderTexture3;

	GatesEngine::RenderTexture blurPlusParticleTex;

	int battleCount;
	EnemyManager enemyManager;
	GameStateManager gameState;
	BossBehaviour* bossBehaviour;

	//GatesEngine::ComputePipeline* gpuParticleCS;
	//ID3D12Resource* particlesData;
	//ID3D12Resource* particlesDrawData;
	//ID3D12Resource* worldData;
	//ID3D12Resource* emittersData;
	struct GaussFilterData
	{
		GatesEngine::Math::Vector4 data[16];
	};
	GaussFilterData gaussData[6];

	GatesEngine::RenderTexture blurRenderTextures[6];
	GatesEngine::RenderTexture reverceResoTextures[3];
private:
	// ガウシアンフィルタの重みを計算する関数
	float GaussFilter(const GatesEngine::Math::Vector2& pos, float value);
	// 参照ピクセルへの情報とガウシアンフィルタの重みを計算してセットする関数
	void SetGaussFilterData(const GatesEngine::Math::Vector2& size, GatesEngine::Math::Vector2& dir, float gaussValue,GaussFilterData* data,int dataSize);
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

