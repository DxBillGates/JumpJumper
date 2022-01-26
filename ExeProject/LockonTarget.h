#pragma once
#include "Header/GameObject/GameObject.h"

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

