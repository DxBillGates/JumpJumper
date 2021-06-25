#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include "Keyboard.h"
#include "Mouse.h"

#include <dinput.h>
#include <Xinput.h>

namespace GatesEngine
{
	//キーボードやマウスといった入力関係を管理するシングルトンクラス
	class Input
	{
	private:
		IDirectInput8* dInput;
		Keyboard keyboard;
		Mouse mouse;
		//XCtrler xctrler;
		//DCtrler dctrler;
	public:
		static Input* GetInstance();
		void Create(HWND hwnd,HINSTANCE hInstance);
		void Initialize();
		void Update();
		Keyboard* GetKeyboard();
		Mouse* GetMouse();
		//XCtrler* GetXInputCtrler();
		//DCtrler* GetDInputCtrler();
	protected:
		Input();
		Input(const Input&) = delete;
		~Input() = default;
		Input& operator=(const Input&) = delete;
	};
}