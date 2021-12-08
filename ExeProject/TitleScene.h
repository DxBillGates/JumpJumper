#pragma once
#include "Header/Scene/Scene.h"
#include "Header/Graphics/Camera3D.h"
#include "Header/Input/Input.h"
#include "TitleManager.h"
#include "Header/Graphics/RenderTexture.h"
#include "Header/Graphics/DepthStencilTexture.h"

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
	float black;
	bool isIncreaseBlack;
	bool isDecreaseBlack;
	bool isSceneTransition;
	ButtonUIType sceneTransType;

	GatesEngine::RenderTexture sceneRenderTexture;
	GatesEngine::DepthStencilTexture sceneDepthTexture;
public:
	TitleScene();
	TitleScene(const char* sceneName);
	TitleScene(const char* sceneName,GatesEngine::Application* app);
	void Initialize() override;
	void Update() override;
	void Draw() override;
	void LateDraw() override;
};
