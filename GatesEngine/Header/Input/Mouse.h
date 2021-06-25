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
		Mouse();
		~Mouse();
		void Create(IDirectInput8* input,HWND hwnd);
		void Initialize();
		void Update();
		bool GetCheckHitButtonAll();
		bool GetCheckHitButton(MouseButtons button);
		bool GetCheckPressTrigger(MouseButtons button);
		bool GetCheckReleaseTrigger(MouseButtons button);
		float GetWheelValue();
		Vector2 GetMouseMove();
		Vector2 GetMousePos();
		void SetMouseCursor(const Vector2& setPos);
	};
}
