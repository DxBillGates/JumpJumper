#pragma once
#include "TutorialState.h"
#include "TutorialUI.h"
#include "Header/Graphics/GraphicsDevice.h"
#include <vector>

class TutorialUIManager
{
private:
	struct TutorialUIInfo
	{
		TutorialState state;
		std::vector<TutorialUI> uies;
	};
private:
	TutorialState currentState;
	std::vector<TutorialUIInfo> tutorialUI;
	int clearUIoffset;
public:
	TutorialUIManager(GatesEngine::GraphicsDevice* device);
	void Initialize();
	void Update(int currentClearCount,bool isAnimation);
	void Draw();
	void SetCurrentState(TutorialState currentState);
	TutorialUI* GetCurrentUIInfo(int currentClearCount);
};

