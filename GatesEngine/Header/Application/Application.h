#pragma once
#include "..\..\Header\Math\Vector2.h"
#include "..\..\Header\Util\Timer.h"
#include "..\Window\Window.h"
#include "..\Input\Input.h"
#include "..\..\Header\Scene\SceneManager.h"
#include "..\..\Header\GameObject\GameObjectManager.h"
#include "..\Graphics\GraphicsDevice.h"
#include "..\Graphics\CBuffer.h"
#include "..\Graphics\Camera.h"

namespace GatesEngine
{
	class Application
	{
	private:
	protected:
		Util::Timer timer;
		Window mainWindow;
		Input* input;
		SceneManager* sceneManager;
		GameObjectManager gameObjectManager;
		GraphicsDevice graphicsDevice;
		CBuffer<B3> worldLightInfo;

		Camera* mainCamera;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Application();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="wSize">Windowのサイズ</param>
		/// <param name="title">Windowのタイトル</param>
		Application(const Math::Vector2& wSize, const char* title);

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Application();

		/// <summary>
		/// アセットのロードをする仮想関数
		/// </summary>
		/// <returns>成否</returns>
		virtual bool LoadContents();

		/// <summary>
		/// アプリケーションの初期化を行う仮想関数
		/// </summary>
		/// <returns>成否</returns>
		virtual bool Initialize();

		/// <summary>
		/// アプリケーションの更新を行う仮想関数
		/// </summary>
		/// <returns>成否</returns>
		virtual bool Update();

		/// <summary>
		/// アプリケーションの描画を行う仮想関数
		/// </summary>
		/// <returns>成否</returns>
		virtual bool Draw();

		/// <summary>
		/// LoadContents、Initializeを一度だけ行い毎フレームUpdateとDrawを行う仮想関数
		/// </summary>
		virtual void Run();

		/// <summary>
		/// ゲーム内経過時間やフレーム時間を保持するクラスのポインタを返す関数
		/// </summary>
		/// <returns>Timerのポインタを返す</returns>
		Util::Timer* GetTimer();

		/// <summary>
		/// シーンマネージャーのポインタを返す関数
		/// </summary>
		/// <returns>シーンマネージャーのポインタを返す</returns>
		SceneManager* GetSceneManager();

		/// <summary>
		/// GraphicsDeviceのポインタを返す関数
		/// </summary>
		/// <returns>GraphicsDeviceのポインタを返す</returns>
		GraphicsDevice* GetGraphicsDevice();

		/// <summary>
		/// このアプリケーションで使用しているWindowの情報を保持したクラスのポインタを返す関数
		/// </summary>
		/// <returns>使用しているWindowの情報を保持したクラスのポインタ</returns>
		Window* GetWindow();

		/// <summary>
		/// キーボードやマウスの情報を保持したクラスのポインタを返す関数
		/// </summary>
		/// <returns>キーボードやマウスの情報を保持したクラスのポインタを返す</returns>
		Input* GetInput();

		/// <summary>
		/// GraphicsDeviceの設定されているカメラのポインタを返す関数
		/// </summary>
		/// <returns>使っているカメラのポインタを返す</returns>
		Camera* GetMainCamera();
	};
}