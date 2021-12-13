#include "GameState.h"
#include <stdio.h>

GameStateManager::GameStateManager()
	: currentState(GameState::FIRST_NORMAL_BATTLE)
	, changeFlag(false)
	, changingTime(0)
	, MAX_CHANGE_TIME(2)
{
}

GameStateManager::~GameStateManager()
{
}

void GameStateManager::Initialize()
{
	currentState = GameState::FIRST_NORMAL_BATTLE;
	changeFlag = false;
	changingTime = 0;
}

void GameStateManager::Update(float deltaTime)
{
	if (!changeFlag)return;

	// ���Ԑ��䏈��
	if (changingTime > MAX_CHANGE_TIME)
	{
		changeFlag = false;
		changingTime = 0;
	}

	changingTime += deltaTime;
}

GameState GameStateManager::GetCurrentState()
{
	return currentState;
}

bool GameStateManager::IsChangingState()
{
	return changeFlag;
}

void GameStateManager::ChangeState()
{
	// �X�e�[�g�ύX�t���O�𗧂Ă�GameStateManager�N���X�̎��Ԃ𓮂���
	changeFlag = true;

	// ���݂̃o�g���X�e�[�g�ɂ��������̃X�e�[�g�Ɉȍ~
	switch (currentState)
	{
	case GameState::FIRST_NORMAL_BATTLE:
		currentState = GameState::FISRT_BOSS_BATTLE;
		break;
	case GameState::FISRT_BOSS_BATTLE:
		currentState = GameState::NORMAL_BATTLE;
		break;
	case GameState::NORMAL_BATTLE:
		currentState = GameState::BOSS_BATTLE;
		break;
	case GameState::BOSS_BATTLE:
		currentState = GameState::NORMAL_BATTLE;
		break;
	default:
		break;
	}
}
