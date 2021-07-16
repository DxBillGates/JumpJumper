#pragma once
#include "Header/Scene/Scene.h"
class TitleScene : public GatesEngine::Scene
{
private:
	const GatesEngine::Math::Vector3 titlePos;
	bool sceneTrasFlag;
	float sceneTrasTime;
public:
	TitleScene();
	TitleScene(const char* sceneName);
	TitleScene(const char* sceneName, GatesEngine::Application* app);
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();
};