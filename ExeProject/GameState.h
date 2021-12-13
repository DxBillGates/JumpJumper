#pragma once

enum class GameState
{
	FIRST_NORMAL_BATTLE,
	FISRT_BOSS_BATTLE,
	NORMAL_BATTLE,
	BOSS_BATTLE,
};

class GameStateManager
{
private:
	GameState currentState;
	bool changeFlag;
	float changingTime;
	const float MAX_CHANGE_TIME;
public:
	GameStateManager();
	~GameStateManager();
	void Initialize();
	void Update(float deltaTime);

	// ���݂̃X�e�[�g��Ԃ��֐�
	GameState GetCurrentState();

	// �Q�[���X�e�[�g���ύX�����ǂ����Ԃ��֐�
	bool IsChangingState();

	// �Q�[���X�e�[�g�����݂̃X�e�[�g�ɂ������X�e�[�g�ɕύX����֐�
	void ChangeState();
};
