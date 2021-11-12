#pragma once
#include "..\GameObject\GameObjectManager.h"
#include "..\Collision\CollisionManager.h"

namespace GatesEngine
{
	class Application;
	class GraphicsDevice;
	class Scene
	{
	protected:
		Application* app;
		GraphicsDevice* graphicsDevice;
		GameObjectManager gameObjectManager;
		CollisionManager collisionManager;
		const char* sceneName;
		bool changeNextSceneFlag;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Scene();

		/// <summary>
		/// シーンに対して名前を設定するコンストラクタ
		/// </summary>
		/// <param name="sceneName">名前</param>
		Scene(const char* sceneName);

		/// <summary>
		/// シーンに対して名前とこのアプリケーションのポインタを設定するコンストラクタ
		/// </summary>
		/// <param name="sceneName">名前</param>
		/// <param name="app">アプリケーションのポインタ</param>
		Scene(const char* sceneName, Application* app);

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~Scene();

		/// <summary>
		/// シーンの初期化をするの純粋仮想関数
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// シーンの更新をするの純粋仮想関数
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// シーンの描画をするの純粋仮想関数
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// シーンの遅延描画をするの仮想関数
		/// </summary>
		virtual void LateDraw() {}

		/// <summary>
		/// シーンの名前を返す関数
		/// </summary>
		/// <returns>シーンの名前</returns>
		const char* GetSceneName();

		/// <summary>
		/// シーンを次のシーンに変更するか設定する関数
		/// </summary>
		/// <returns>成否</returns>
		bool IsChangeScene();

		/// <summary>
		/// シーンに存在するゲームオブジェクトマネージャーのポインタを返す関数
		/// </summary>
		/// <returns>ゲームオブジェクトマネージャーのポインタ</returns>
		GameObjectManager* GetGameObjectManager();
	};
}
