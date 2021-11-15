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
		/// コンストラクタ
		/// </summary>
		Window();
		~Window();

		/// <summary>
		/// MainWindow生成関数
		/// </summary>
		/// <param name="windowSize">ウィンドウサイズ</param>
		/// <param name="title">ウィンドウタイトル</param>
		/// <returns>成否</returns>
		bool Create(const Vector2& windowSize, const char* title);

		/// <summary>
		/// MainWindow表示
		/// </summary>
		void PreviewWindow();

		/// <summary>
		/// Windowsへの応答などする関数
		/// </summary>
		/// <returns>成否</returns>
		bool ProcessMessage();

		/// <summary>
		/// MainWindowのハンドルを返す関数
		/// </summary>
		/// <returns>MainWindowのハンドル</returns>
		HWND GetHandle();

		/// <summary>
		/// MainWindowのインスタンスハンドルを返す関数
		/// </summary>
		/// <returns>MainWindowのインスタンスハンドル</returns>
		HINSTANCE GetHInstance();

		/// <summary>
		/// MainWindowのサイズを返す関数
		/// </summary>
		/// <returns>MainWindowのサイズ</returns>
		Vector2 GetWindowSize();

		/// <summary>
		/// MainWindowのアスペクト比を返す関数
		/// </summary>
		/// <returns>MainWindowのアスペクト比</returns>
		float GetWindowAspect();

		/// <summary>
		/// MainWindowの左上の座標を設定する関数
		/// </summary>
		/// <param name="pos">位置(pixel)</param>
		void SetWindowPos(const Vector2& pos);

		/// <summary>
		/// MainWindowのタイトルを変更する関数
		/// </summary>
		/// <param name="changeTitle">設定する名前</param>
		void SetWindowTitle(const char* changeTitle);
	};
}
