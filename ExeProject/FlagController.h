#pragma once

/// <summary>
/// ゲーム内で使用するフラグを使いやすくしたクラス
/// </summary>
class FlagController
{
protected:
	// フラグ
	bool flag;
	// フラグがオンになってからの経過時間
	float elapsedTime;
	// 前フレームの経過時間
	float preElapsedTime;

	// 上限タイム
	float PROPERTY_MAX_TIME;
public:
	FlagController();
	virtual ~FlagController();
	virtual void Initialize();
	virtual void Update(float deltaTime);

	// 経過時間が1を超えたフレームのみtrueを返す
	bool GetOverTimeTrigger();
	bool GetFlag();
	float GetTime();
	void SetFlag(bool setFlag);
	void SetTime(float setValue);
	void SetMaxTimeProperty(const float value);

	bool IsOverTime(float value = 1);
	bool IsUnderTime(float value = 1);
};

