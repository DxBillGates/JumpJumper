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

	// 現在のステートを返す関数
	GameState GetCurrentState();

	// ゲームステートが変更中かどうか返す関数
	bool IsChangingState();

	// ゲームステートを現在のステートにあったステートに変更する関数
	void ChangeState();
};
