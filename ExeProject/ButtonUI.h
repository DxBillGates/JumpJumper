#pragma once
#include "Header/Math/Vector2.h"

enum class ButtonUIType
{
	START,
	EXIT,
};

/// <summary>
/// 2DUIに使うクラス
/// </summary>
class ButtonUI
{
protected:
	using Vector2 = GatesEngine::Math::Vector2;

	Vector2 scale;
	Vector2 pos;

	bool isCenter;
	bool isSelect;

	float alpha;

	ButtonUIType type;
public:

	/// <summary>
	/// ボタンの大きさを返す関数
	/// </summary>
	/// <returns>ボタンの大きさ</returns>
	const Vector2& GetScale();

	/// <summary>
	/// ボタンの位置を返す関数
	/// </summary>
	/// <returns>ボタンの位置</returns>
	const Vector2& GetPos();

	/// <summary>
	/// ボタンの基点が左上か中心か確認する関数
	/// </summary>
	/// <returns>成否</returns>
	bool GetIsCenter();

	/// <summary>
	/// ボタンが選択されているか確認する関数
	/// </summary>
	/// <returns>成否</returns>
	bool GetIsSelect();

	/// <summary>
	/// UIの透明度を取得する関数
	/// </summary>
	/// <returns>UIの透明度</returns>
	float GetAlpha();

	/// <summary>
	/// ボタンの種類を取得する関数
	/// </summary>
	/// <returns>ボタンの種類</returns>
	ButtonUIType GetType();

	/// <summary>
	/// ボタンの大きさを設定する関数
	/// </summary>
	/// <param name="setscale">設定する大きさ</param>
	void SetScale(const Vector2& setscale);

	/// <summary>
	/// ボタンの位置を設定する関数
	/// </summary>
	/// <param name="setpos">設定する位置</param>
	void SetPos(const Vector2& setpos);

	/// <summary>
	/// ボタンの基点を左上にするか中心にするか設定する関数
	/// </summary>
	/// <param name="flag">成否</param>
	void SetIsCenterFlag(bool flag);

	/// <summary>
	/// ボタンが選択されているか設定する関数
	/// </summary>
	/// <param name="flag">成否</param>
	void SetIsSelect(bool flag);

	/// <summary>
	/// UIの透明度を設定する関数
	/// </summary>
	/// <param name="value">設定値</param>
	void SetAlpha(float value);

	/// <summary>
	/// ボタンの種類を設定する関数
	/// </summary>
	/// <param name="buttonType">設定するタイプ</param>
	void SetType(ButtonUIType buttonType);
};

