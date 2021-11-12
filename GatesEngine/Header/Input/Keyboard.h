#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Keys.h"
#include <dinput.h>

namespace GatesEngine
{
	class Keyboard
	{
	private:
		IDirectInputDevice8* device;
		BYTE currentKeys[256];
		BYTE beforeKeys[256];
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Keyboard();
		~Keyboard();

		/// <summary>
		/// デバイスを生成する関数
		/// </summary>
		/// <param name="input">DInputの親デバイスのポインタ</param>
		/// <param name="hwnd">使用しているWindowのハンドル</param>
		void Create(IDirectInput8* input, HWND hwnd);

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update();

		/// <summary>
		/// 指定したキーが現在のフレームで初めて押されたか確認する関数
		/// </summary>
		/// <param name="key">指定キー</param>
		/// <returns>成否</returns>
		bool CheckPressTrigger(Keys key);

		/// <summary>
		/// 指定したキーが現在のフレームで初めて離されたか確認する関数
		/// </summary>
		/// <param name="key">指定キー</param>
		/// <returns>成否</returns>
		bool CheckReleaseTrigger(Keys key);

		/// <summary>
		/// 指定したキーが押されているか確認する関数
		/// </summary>
		/// <param name="key">指定キー</param>
		/// <returns>成否</returns>
		bool CheckHitKey(Keys key);

		/// <summary>
		/// 何かしらのキーが押されているか確認する関数
		/// </summary>
		/// <returns>成否</returns>
		bool CheckHitKeyAll();
	};
}

