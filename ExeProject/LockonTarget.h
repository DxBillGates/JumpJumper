#pragma once
#include "Header/GameObject/GameObject.h"

#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

class LockonTarget
{
private:
	// ���b�N�I�����Ă�Q�[���I�u�W�F�N�g
	GatesEngine::GameObject* target;
	// ���b�N�I�����Ă���̌o�ߎ���
	float lockonTime;
	// ���b�N�I�����Ă���̍ő厞��
	const int MAX_LOCKON_TIME;
	// ���b�N���Ă��邩
	bool isLockon;

	bool wasLockon;

	float scaleAnimationTime;
	float angle;
	float depth;
public:
	LockonTarget();
	~LockonTarget();

	LockonTarget& operator=(const LockonTarget& other);

	void Initialize();
	void BaseDataInitialize();
	void AnimationDataInitialize();
	void Update();
	void Draw(GatesEngine::GraphicsDevice* gDevice,const GatesEngine::Math::Matrix4x4& billBoardData);

	void SetLockonTarget(GatesEngine::GameObject* target);
	void SetWasLockon(bool flag);
	GatesEngine::GameObject* GetTarget();

	float GetLockTime();
	const bool GetIsLockon() const;
	const bool GetWasLockon() const;
	const int GetMaxLockonTime();

	const float GetDepth() const;
	void SetDepth(float value);
};

