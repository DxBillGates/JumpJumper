#pragma once
#include "Header/GameObject/GameObject.h"

class LockonTarget
{
private:
	// ロックオンしてるゲームオブジェクト
	GatesEngine::GameObject* target;
	// ロックオンしてからの経過時間
	float lockonTime;
	// ロックオンしてからの最大時間
	const int MAX_LOCKON_TIME;
	// ロックしているか
	bool isLockon;
public:
	LockonTarget();
	~LockonTarget();
	void Initialize();
	void Update();
	void SetLockonTarget(GatesEngine::GameObject* target);
	GatesEngine::GameObject* GetTarget();
	float GetLockTime();
	bool GetIsLockon();
	const int GetMaxLockonTime();
};

