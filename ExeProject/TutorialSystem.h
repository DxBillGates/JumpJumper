#pragma once
#include "TutorialState.h"
#include "TutorialUIManager.h"
#include "TutorialTask.h"
#include "PlayerBehaviour.h"
#include "Header/Graphics/GraphicsDevice.h"

class TutorialSystem
{
private:
	TutorialState currentState;
	bool isUIScaleAnimation;
	bool transStateFlag;
	int currentStateClearCount;

	TutorialUIManager tutorialUIManager;
	std::vector<TutorialTask*> tutorialTasks;

	bool transStateIntervalFlag;
	float transStateIntervalTime;

	bool preEndTutorialFlag;
	float endTutorialTime;
	const float MAX_END_TUTORIAL_TIME;

	bool endTutorialFlag;
public:
	TutorialSystem(GatesEngine::GraphicsDevice* device);
	~TutorialSystem();
	void Initialize();
	void Update(bool sceneTransFlag = false);
	void Draw();
	void AddClearCount();
	void TransCurrentState();
	void ChangeCurrentState(TutorialState setState);
	void SetPlayerBehaviour(PlayerBehaviour* player);
	bool GetEndTutorialFlag();
};

