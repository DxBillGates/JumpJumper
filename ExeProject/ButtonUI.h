#pragma once
#include "Header/Math/Vector2.h"

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
};

