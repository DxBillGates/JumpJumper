#pragma once
#include "TutorialTask.h"
#include "PlayerBehaviour.h"
class BoostMoveTask : public TutorialTask
{
private:
	PlayerBehaviour* player;
public:
	BoostMoveTask();
	~BoostMoveTask();
	void Initialize() override;
	void Update() override;
	void SetPlayer(PlayerBehaviour* behaviour);
};
