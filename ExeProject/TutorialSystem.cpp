#include "TutorialSystem.h"
#include "NormalAttackTask.h"

TutorialSystem::TutorialSystem(GatesEngine::GraphicsDevice* device)
	: currentState(TutorialState::NORMAL_ATTACK)
	, isUIScaleAnimation(false)
	, transStateFlag(false)
	, currentStateClearCount(0)
	, tutorialUIManager(device)
{
	tutorialTasks.push_back(new NormalAttackTask());
}

TutorialSystem::~TutorialSystem()
{
	for (auto& task : tutorialTasks)
	{
		delete task;
	}

	tutorialTasks.clear();
}

void TutorialSystem::Initialize()
{
	tutorialUIManager.Initialize();

	for (auto& task : tutorialTasks)
	{
		task->Initialize();
	}
}

void TutorialSystem::Update()
{
	if (transStateIntervalTime >= 1)
	{
		transStateIntervalFlag = false;
		transStateIntervalTime = 0;
		TransCurrentState();
	}

	if (transStateIntervalFlag)
	{
		const float PER_FRAME = 1.0f / 60.0f;
		const float INTERVAL = 10;
		transStateIntervalTime += PER_FRAME / INTERVAL;
	}

	int currentIntState = (int)currentState;
	if (currentIntState < (int)tutorialTasks.size() && !transStateIntervalFlag)
	{
		if (tutorialTasks[currentIntState]->IsClearTask())
		{
			transStateIntervalFlag = true;
			//TransCurrentState();
		}
		if (tutorialTasks[currentIntState]->GetClearFlag())
		{
			AddClearCount();
		}
		tutorialTasks[currentIntState]->Update();
	}

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

void TutorialSystem::SetPlayerBehaviour(PlayerBehaviour* player)
{
	if ((int)tutorialTasks.size() <= 0)return;

	NormalAttackTask* normalAttackTask = dynamic_cast<NormalAttackTask*>(tutorialTasks[0]);
	if (!normalAttackTask)return;
	normalAttackTask->SetPlayer(player);
}
