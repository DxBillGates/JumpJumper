#pragma once
#include "Header/Scene/Scene.h"
class SampleScene : public GatesEngine::Scene
{
private:
public:
	SampleScene();
	SampleScene(const char* sceneName);
	SampleScene(const char* sceneName, GatesEngine::Application* app);
	~SampleScene();
	void Initialize();
	void Update();
	void Draw();
};
