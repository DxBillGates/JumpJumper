#include "EmitteAttackTask.h"

EmitteAttackTask::EmitteAttackTask()
	: player(nullptr)
{
}

EmitteAttackTask::~EmitteAttackTask()
{
}

void EmitteAttackTask::Initialize()
{
	if (!player)errorTaskFlag = true;
}

void EmitteAttackTask::Update()
{
	ErrorTaskCheck();

	preClearFlag = clearFlag;
	clearFlag = false;

	if (player->GetEmitteAttackFlag())
	{
		IncrementClearTaskCount();
		clearFlag = true;
	}
}

void EmitteAttackTask::SetPlayer(PlayerBehaviour* behaviour)
{
	player = behaviour;
}
