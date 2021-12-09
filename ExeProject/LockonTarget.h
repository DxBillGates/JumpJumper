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

