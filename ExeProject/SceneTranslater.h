#pragma once
#include "Header/Graphics/Graphics.h"
#include "Header/Math/Math.h"

class SceneTranslater
{
public:
	enum class TranslateState
	{
		NONE,
		UP,
		DOWN,
	};
private:
	int setTime;
	float time;
	GatesEngine::Math::Vector3 scale;
	TranslateState state;
	bool isActive;
public:
	SceneTranslater();
	void StartSceneTranslate(int time);
	void Update(float elapsedTime);
	void Draw(GatesEngine::GraphicsDevice* graphicsDevice);
	bool IsActive();
	void SetIsActive(bool setFlag);
	void SetTranslateState(TranslateState setState);
	TranslateState GetTranslateState();
};

