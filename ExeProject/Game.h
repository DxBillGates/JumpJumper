#pragma once
#include "Header/Application/Application.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Shader.h"
#include "Header/Graphics/CBuffer.h"
#include "Header/Graphics/Mesh.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"

class Game : public GatesEngine::Application
{
private:
	GatesEngine::RenderTexture shadowRenderTex;
	GatesEngine::DepthStencilTexture shadowDepthTex;

	GatesEngine::RenderTexture resultRenderTex;
	GatesEngine::DepthStencilTexture resultDepthTex;

	GatesEngine::RenderTexture lateDrawResultRenderTex;
	GatesEngine::DepthStencilTexture lateDrawResultDepthTex;

	GatesEngine::RenderTexture resultRenderShadowTex;
public:
	Game();
	Game(const GatesEngine::Math::Vector2& wSize, const char* title);
	~Game();
	bool LoadContents()override;
	bool Initialize()override;
	bool Update()override;
	bool Draw()override;
};
