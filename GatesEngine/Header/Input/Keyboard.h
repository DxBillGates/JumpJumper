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
		Keyboard();
		~Keyboard();
		void Create(IDirectInput8* input, HWND hwnd);
		void Initialize();
		void Update();
		bool CheckPressTrigger(Keys key);
		bool CheckReleaseTrigger(Keys key);
		bool CheckHitKey(Keys key);
		bool CheckHitKeyAll();
	};
}

