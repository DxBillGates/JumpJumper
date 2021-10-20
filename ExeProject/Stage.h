#pragma once
#include "GameObjectCollisionManager.h"

class Stage
{
protected:
	int needKillCount;
	GameObjectCollisionManager gCollisionManager;
public:
	Stage();
	virtual ~Stage();
	int GetNeedKillCount();
	void Update();
	GameObjectCollisionManager* GetCollisionManager();
};
