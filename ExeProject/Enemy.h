#pragma once
#include "Header/Math/Vector3.h"
class Enemy
{
protected:
	GatesEngine::Math::Vector3 iPos;
	GatesEngine::Math::Vector3 pos;
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Math::Vector3 acc;
	GatesEngine::Math::Vector3 target;
	float setTime;
	bool isUse;
	bool isSetting;
	bool isTarget;

	bool isSpawning;
	float spawnTime;
public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();

	/// <summary>
	/// 座標を取得する関数
	/// </summary>
	/// <returns>座標</returns>
	GatesEngine::Math::Vector3 GetPosition();

	/// <summary>
	/// 使用中か確認する関数
	/// </summary>
	/// <returns>成否</returns>
	bool GetIsUse();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>成否</returns>
	bool GetIsSetting();

	/// <summary>
	/// ターゲットが設定されているか確認する関数
	/// </summary>
	/// <returns>成否</returns>
	bool GetIsTarget();

	/// <summary>
	/// 位置を設定する関数
	/// </summary>
	/// <param name="pos">セットする座標</param>
	void SetPosition(const GatesEngine::Math::Vector3& pos);

	/// <summary>
	/// メンバ変数のvelを設定する関数
	/// </summary>
	/// <param name="vel">設定する値</param>
	void SetVelocity(const GatesEngine::Math::Vector3& vel);

	/// <summary>
	/// 設定にかける時間を設定する
	/// </summary>
	/// <param name="value">設定する値</param>
	void SetTime(float value);

	/// <summary>
	/// 使用しているかを確認する変数を設定する関数
	/// </summary>
	/// <param name="flag">設定する値</param>
	void SetIsUse(bool flag);

	/// <summary>
	/// ホーミングのような挙動を設定する関数
	/// </summary>
	/// <param name="targetPos"></param>
	void SetTarget(const GatesEngine::Math::Vector3& targetPos, const GatesEngine::Math::Vector3& addVector);
};

