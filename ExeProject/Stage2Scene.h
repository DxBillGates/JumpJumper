#pragma once
#include "Header/Scene/Scene.h"
#include "Stage2.h"
#include "SceneTranslater.h"
#include "PlayerBehaviour.h"
class Stage2Scene : public GatesEngine::Scene
{
private:
	Stage2 stage;
	SceneTranslater sceneTranslater;
	PlayerBehaviour* playerBehaviour;
public:
	Stage2Scene();
	Stage2Scene(const char* sceneName);
	Stage2Scene(const char* sceneName, GatesEngine::Application* app);
	~Stage2Scene();
	void Initialize();
	void Update();
	void Draw();
};

