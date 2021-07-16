#pragma once
#include "Stage.h"
#include "PlayerBehaviour.h"
#include <vector>

class StageManager
{
private:
	std::vector<Stage*> stages;
	Stage* currentStage;
	PlayerBehaviour* player;
public:
	StageManager();
	~StageManager();
	void Update();
	void Add(Stage* newStage);
	Stage* GetCurrentStage();
	void ChangeStage(int stageNumber);
	void ChangeNextStage();
	void SetPlayer(PlayerBehaviour* playerBehaviour);
};
