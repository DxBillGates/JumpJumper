#include "NormalAttackTask.h"

NormalAttackTask::NormalAttackTask()
	: player(nullptr)
{
}

NormalAttackTask::~NormalAttackTask()
{
}

void NormalAttackTask::Initialize()
{
	if (!player)errorTaskFlag = true;
}

void NormalAttackTask::Update()
{
	ErrorTaskCheck();

	preClearFlag = clearFlag;
	clearFlag = false;

	if (player->GetNormalAttackFlag())
	{
		IncrementClearTaskCount();
		clearFlag = true;
	}
}

void NormalAttackTask::SetPlayer(PlayerBehaviour* behaviour)
{
	player = behaviour;
}
