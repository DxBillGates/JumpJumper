#pragma once
#include "Header/Scene/Scene.h"
#include "SceneTranslater.h"
#include "Header/Graphics/Camera.h"
#include "Header/Input/Input.h"

class SelectScene :  public GatesEngine::Scene
{
private:
	enum class SelectState
	{
		NEXT_STAGE,
		TITLE,
	};
	GatesEngine::Camera* mainCamera;
	SceneTranslater sceneTranslater;
	SelectState selectState;
	GatesEngine::Input* input;
public:
	SelectScene();
	SelectScene(const char* sceneName);
	SelectScene(const char* sceneName, GatesEngine::Application* app);
	~SelectScene();
	void Initialize();
	void Update();
	void Draw();
};

