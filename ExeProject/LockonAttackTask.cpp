#include "LockonAttackTask.h"

LockonAttackTask::LockonAttackTask()
	: player(nullptr)
{
}

LockonAttackTask::~LockonAttackTask()
{
}

void LockonAttackTask::Initialize()
{
	if (!player)errorTaskFlag = true;
	TutorialTask::Initialize();
}

void LockonAttackTask::Update()
{
	ErrorTaskCheck();

	preClearFlag = clearFlag;
	clearFlag = false;

	if (player->GetLockonAttackFlag())
	{
		IncrementClearTaskCount();
		clearFlag = true;
	}
}

void LockonAttackTask::SetPlayer(PlayerBehaviour* behaviour)
{
	player = behaviour;
}
