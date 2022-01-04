#include "BoostMoveTask.h"

BoostMoveTask::BoostMoveTask()
	: player(nullptr)
{
}

BoostMoveTask::~BoostMoveTask()
{
}

void BoostMoveTask::Initialize()
{
	if (!player)errorTaskFlag = true;
	TutorialTask::Initialize();
}

void BoostMoveTask::Update()
{
	ErrorTaskCheck();

	preClearFlag = clearFlag;
	clearFlag = false;

	if (player->GetBoostMoveFlag())
	{
		IncrementClearTaskCount();
		clearFlag = true;
	}
}

void BoostMoveTask::SetPlayer(PlayerBehaviour* behaviour)
{
	player = behaviour;
}
