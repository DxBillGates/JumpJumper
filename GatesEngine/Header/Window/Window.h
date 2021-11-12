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
		/// <summary>
		/// 
		/// </summary>
		Window();
		~Window();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="windowSize"></param>
		/// <param name="title"></param>
		/// <returns></returns>
		bool Create(const Vector2& windowSize, const char* title);

		/// <summary>
		/// 
		/// </summary>
		void PreviewWindow();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool ProcessMessage();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		HWND GetHandle();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		HINSTANCE GetHInstance();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Vector2 GetWindowSize();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		float GetWindowAspect();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pos"></param>
		void SetWindowPos(const Vector2& pos);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="changeTitle"></param>
		void SetWindowTitle(const char* changeTitle);
	};
}
