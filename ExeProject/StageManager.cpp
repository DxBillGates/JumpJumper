#include "StageManager.h"

StageManager::StageManager()
{
}

StageManager::~StageManager()
{
	for (int i = 0; i < (int)stages.size(); ++i)
	{
		delete stages[i];
	}
}

void StageManager::Update()
{
	isChangeScene = false;
	if (player && currentStage)
	{
		if (player->GetKillCount() >= currentStage->GetNeedKillCount())
		{
			ChangeNextStage();
			player->ResetKillCount();
		}
	}
}

void StageManager::Add(Stage* newStage)
{
	stages.push_back(newStage);
}

Stage* StageManager::GetCurrentStage()
{
	return currentStage;
}

void StageManager::ChangeStage(int stageNumber)
{
	if (stageNumber >= stages.size())return;

	currentStage = stages[stageNumber];
	isChangeScene = true;
}

void StageManager::ChangeNextStage()
{
	for (int i = 0; i < (int)stages.size(); ++i)
	{
		if (currentStage == stages[i])
		{
			if (i + 1 >= (int)stages.size())return;
			currentStage = stages[i + 1];
			isChangeScene = true;
			break;
		}
	}
}

void StageManager::SetPlayer(PlayerBehaviour* playerBehaviour)
{
	player = playerBehaviour;
}

bool StageManager::IsChangeStage()
{
	return isChangeScene;
}
