#include "TutorialSystem.h"

TutorialSystem::TutorialSystem(GatesEngine::GraphicsDevice* device)
	: currentState(TutorialState::NORMAL_ATTACK)
	, isUIScaleAnimation(false)
	, transStateFlag(false)
	, currentStateClearCount(0)
	, tutorialUIManager(device)
{
}

void TutorialSystem::Initialize()
{
	tutorialUIManager.Initialize();
}

void TutorialSystem::Update()
{
	//if (transStateFlag)
	//{
	//	int nextState = (int)currentState + 1;
	//	if (nextState < (int)TutorialState::END)
	//	{
	//		currentState = (TutorialState)(nextState);
	//	}
	//}

	//if (!isUIScaleAnimation)
	//{
	//	if (!tutorialUIManager.GetCurrentUIInfo(currentStateClearCount)->GetScaleAnimationFlag())
	//	{
	//		transStateFlag = true;
	//	}
	//}
	tutorialUIManager.SetCurrentState(currentState);
	tutorialUIManager.Update(currentStateClearCount, isUIScaleAnimation);
	isUIScaleAnimation = false;
}

void TutorialSystem::Draw()
{
	tutorialUIManager.Draw();
}

void TutorialSystem::AddClearCount()
{
	isUIScaleAnimation = true;
	++currentStateClearCount;
}

void TutorialSystem::TransCurrentState()
{
	int nextState = (int)currentState + 1;
	if (nextState < (int)TutorialState::END)
	{
		currentState = (TutorialState)(nextState);
		currentStateClearCount = 0;
	}
}

void TutorialSystem::ChangeCurrentState(TutorialState setState)
{
	currentState = setState;
}
