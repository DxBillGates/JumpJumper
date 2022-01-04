#include "TutorialSystem.h"
#include "NormalAttackTask.h"
#include "LockonAttackTask.h"
#include "EmitteAttackTask.h"

TutorialSystem::TutorialSystem(GatesEngine::GraphicsDevice* device)
	: currentState(TutorialState::NORMAL_ATTACK)
	, isUIScaleAnimation(false)
	, transStateFlag(false)
	, currentStateClearCount(0)
	, tutorialUIManager(device)
	, transStateIntervalFlag(false)
	, transStateIntervalTime(0)
	, preEndTutorialFlag(false)
	, endTutorialTime(0)
	, MAX_END_TUTORIAL_TIME(10)
	, endTutorialFlag(false)
{
	tutorialTasks.push_back(new NormalAttackTask());
	tutorialTasks.push_back(new LockonAttackTask());
	tutorialTasks.push_back(new EmitteAttackTask());
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

	currentState = TutorialState::NORMAL_ATTACK;
	isUIScaleAnimation = false;
	transStateFlag = false;
	currentStateClearCount = 0;
	transStateIntervalFlag = false;
	transStateIntervalTime = 0;
	preEndTutorialFlag = false;
	endTutorialTime = 0;
	endTutorialFlag = false;
}

void TutorialSystem::Update(bool sceneTransFlag)
{
	if (transStateIntervalTime >= 1)
	{
		transStateIntervalFlag = false;
		transStateIntervalTime = 0;
		TransCurrentState();
	}

	const float PER_FRAME = 1.0f / 60.0f;
	if (transStateIntervalFlag)
	{
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

	if (endTutorialTime > 1)
	{
		Initialize();
		endTutorialFlag = true;
	}
	if (preEndTutorialFlag)
	{
		endTutorialTime += PER_FRAME / MAX_END_TUTORIAL_TIME;
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
	if (!sceneTransFlag)
	{
		tutorialUIManager.SetCurrentState(currentState);
		tutorialUIManager.Update(currentStateClearCount, isUIScaleAnimation,preEndTutorialFlag);
		isUIScaleAnimation = false;
	}
}

void TutorialSystem::Draw()
{
	tutorialUIManager.Draw();
}

void TutorialSystem::AddClearCount()
{
	isUIScaleAnimation = true;
	++currentStateClearCount;
	if (currentStateClearCount > 3)currentStateClearCount = 3;
}

void TutorialSystem::TransCurrentState()
{
	int nextState = (int)currentState + 1;
	if (nextState < (int)TutorialState::END)
	{
		currentState = (TutorialState)(nextState);
		currentStateClearCount = 0;
	}
	else
	{
		preEndTutorialFlag = true;
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

	LockonAttackTask* lockonAttackTask = dynamic_cast<LockonAttackTask*>(tutorialTasks[1]);
	if (!lockonAttackTask)return;
	lockonAttackTask->SetPlayer(player);

	EmitteAttackTask* emitteAttackTask = dynamic_cast<EmitteAttackTask*>(tutorialTasks[2]);
	if (!emitteAttackTask)return;
	emitteAttackTask->SetPlayer(player);
}

bool TutorialSystem::GetEndTutorialFlag()
{
	return endTutorialFlag;
}
