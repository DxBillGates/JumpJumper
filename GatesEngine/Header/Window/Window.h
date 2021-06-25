#pragma once
#include "..\Math\Vector2.h"
#include <Windows.h>

namespace GatesEngine
{
	class Window
	{
		using Vector2 = GatesEngine::Math::Vector2;
	private:
		HWND hwnd;
		WNDCLASSEX wndClass;
		MSG msg;
		Vector2 size;
		Vector2 pos;
	public:
		Window();
		~Window();
		bool Create(const Vector2& windowSize, const char* title);
		void PreviewWindow();
		bool ProcessMessage();

		HWND GetHandle();
		HINSTANCE GetHInstance();
		Vector2 GetWindowSize();
		float GetWindowAspect();
		void SetWindowPos(const Vector2& pos);
		void SetWindowTitle(const char* changeTitle);
	};
}
