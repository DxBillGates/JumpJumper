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

	float scaleAnimationTime;
	float angle;
public:
	LockonTarget();
	~LockonTarget();
	void Initialize();
	void BaseDataInitialize();
	void AnimationDataInitialize();
	void Update();
	void Draw(GatesEngine::GraphicsDevice* gDevice,const GatesEngine::Math::Matrix4x4& billBoardData);

	void SetLockonTarget(GatesEngine::GameObject* target);
	GatesEngine::GameObject* GetTarget();
	float GetLockTime();
	bool GetIsLockon();
	const int GetMaxLockonTime();
};

