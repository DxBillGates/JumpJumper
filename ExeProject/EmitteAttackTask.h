#pragma once
#include "TutorialTask.h"
#include "PlayerBehaviour.h"

class EmitteAttackTask : public TutorialTask
{
private:
	PlayerBehaviour* player;
public:
	EmitteAttackTask();
	~EmitteAttackTask();
	void Initialize() override;
	void Update() override;
	void SetPlayer(PlayerBehaviour* behaviour);
};

