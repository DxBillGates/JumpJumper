#pragma once
#include "TutorialState.h"
#include "TutorialUIManager.h"
#include "Header/Graphics/GraphicsDevice.h"

class TutorialSystem
{
private:
	TutorialState currentState;
	bool isUIScaleAnimation;
	bool transStateFlag;
	int currentStateClearCount;

	TutorialUIManager tutorialUIManager;
public:
	TutorialSystem(GatesEngine::GraphicsDevice* device);
	void Initialize();
	void Update();
	void Draw();
	void AddClearCount();
	void TransCurrentState();
	void ChangeCurrentState(TutorialState setState);
};

