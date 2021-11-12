#pragma once
#include <vector>
#include "Scene.h"
namespace GatesEngine
{
	class SceneManager
	{
	private:
		std::vector<Scene*> scenes;
		Scene* currentScene;
		Scene* beforeScene;
	protected:
	public:
		/// <summary>
		/// シングルトンのインスタンスを生成して返す関数
		/// </summary>
		/// <returns>シングルトンのインスタンスのポインタ</returns>
		static SceneManager* GetInstance();

		/// <summary>
		/// シーンを追加する関数
		/// </summary>
		/// <param name="newScene">追加するシーンの名前</param>
		void AddScene(Scene* newScene);

		/// <summary>
		/// シーンを変更する関数
		/// </summary>
		/// <param name="sceneName">変更先のシーンの名前</param>
		void ChangeScene(const char* sceneName);

		/// <summary>
		/// シーンを変更した後にシーンの初期化を行わない関数
		/// </summary>
		/// <param name="sceneName">変更先のシーンの名前</param>
		void ChangeSceneWithoutInitialize(const char* sceneName);

		/// <summary>
		/// 現在のシーンのポインタを返す関数
		/// </summary>
		/// <returns>現在のシーンのポインタ</returns>
		Scene* GetCurrentScene();

		/// <summary>
		/// 前のシーンのポインタを返す関数
		/// </summary>
		/// <returns>前のシーンのポインタ</returns>
		Scene* GetBeforeScene();

		/// <summary>
		/// 指定したシーンのポインタを返す関数
		/// </summary>
		/// <param name="sceneName">指定するシーンの名前</param>
		/// <returns>指定したシーンのポインタ</returns>
		Scene* GetScene(const char* sceneName);

		/// <summary>
		/// 現在のシーンを初期化する関数
		/// </summary>
		void Initialize();

		/// <summary>
		/// 現在のシーンを更新する関数
		/// </summary>
		void Update();

		/// <summary>
		/// 現在のシーンを描画する関数
		/// </summary>
		void Draw();

		/// <summary>
		/// 現在のシーンを遅延描画する関数
		/// </summary>
		void LateDraw();
	private:
		SceneManager();
		SceneManager(const SceneManager&) = delete;
		~SceneManager();
		SceneManager& operator=(const SceneManager&) = delete;
	};
}
