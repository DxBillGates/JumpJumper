#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "MouseButtons.h"
#include "..\Math\Vector2.h"
#include <dinput.h>

namespace GatesEngine
{
	class Mouse
	{
		using Vector2 = GatesEngine::Math::Vector2;
	private:
		IDirectInputDevice8* device;
		DIMOUSESTATE2 currentMouseState;
		DIMOUSESTATE2 beforeMouseState;
		Vector2 mousePos;
		Vector2 mouseMove;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Mouse();
		~Mouse();

		/// <summary>
		/// デバイスの生成関数
		/// </summary>
		/// <param name="input">DInputの親デバイスのポインタ</param>
		/// <param name="hwnd">使用しているWindowのハンドル</param>
		void Create(IDirectInput8* input,HWND hwnd);

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update();

		/// <summary>
		/// マウスの何かしらのキーが押されているか確認する関数
		/// </summary>
		/// <returns>成否</returns>
		bool GetCheckHitButtonAll();

		/// <summary>
		/// 指定したキーが押されているか確認する関数
		/// </summary>
		/// <param name="button">指定キー</param>
		/// <returns>成否</returns>
		bool GetCheckHitButton(MouseButtons button);

		/// <summary>
		/// 指定したキーが現在のフレームで初めて押されたか確認する関数
		/// </summary>
		/// <param name="button">指定キー</param>
		/// <returns>成否</returns>
		bool GetCheckPressTrigger(MouseButtons button);

		/// <summary>
		/// 指定したキーが現在のフレームで初めて離されたか確認する関数
		/// </summary>
		/// <param name="button">指定キー</param>
		/// <returns>成否</returns>
		bool GetCheckReleaseTrigger(MouseButtons button);

		/// <summary>
		/// ホイールの情報を返す関数
		/// </summary>
		/// <returns>ホイールの情報</returns>
		float GetWheelValue();
		
		/// <summary>
		/// 前フレームからどれだけマウスが動いたか返す関数
		/// </summary>
		/// <returns>動いたピクセル数</returns>
		Vector2 GetMouseMove();

		/// <summary>
		/// マウスカーソルがどのピクセルにいるかを返す関数
		/// </summary>
		/// <returns>ピクセル位置</returns>
		Vector2 GetMousePos();

		/// <summary>
		/// マウスカーソルを指定したピクセルに固定する関数
		/// </summary>
		/// <param name="setPos">指定ピクセル</param>
		void SetMouseCursor(const Vector2& setPos);
	};
}
