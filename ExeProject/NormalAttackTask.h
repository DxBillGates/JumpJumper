#pragma once
#include "TutorialTask.h"
#include "PlayerBehaviour.h"
class NormalAttackTask : public TutorialTask
{
private:
	PlayerBehaviour* player;
public:
	NormalAttackTask();
	~NormalAttackTask();
	void Initialize() override;
	void Update() override;
	void SetPlayer(PlayerBehaviour* behaviour);
};

