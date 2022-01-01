#pragma once
#include "TutorialTask.h"
#include "PlayerBehaviour.h"

class LockonAttackTask : public TutorialTask
{
private:
	PlayerBehaviour* player;
public:
	LockonAttackTask();
	~LockonAttackTask();
	void Initialize() override;
	void Update() override;
	void SetPlayer(PlayerBehaviour* behaviour);
};

