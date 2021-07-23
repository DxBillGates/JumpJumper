#pragma once
#include "Header/Scene/Scene.h"
#include "Stage1.h"
#include "SceneTranslater.h"
#include "PlayerBehaviour.h"
class Stage1Scene : public GatesEngine::Scene
{
private:
	Stage1 stage;
	SceneTranslater sceneTranslater;
	PlayerBehaviour* playerBehaviour;
public:
	Stage1Scene();
	Stage1Scene(const char* sceneName);
	Stage1Scene(const char* sceneName, GatesEngine::Application* app);
	~Stage1Scene();
	void Initialize();
	void Update();
	void Draw();
};

