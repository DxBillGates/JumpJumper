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
		static SceneManager* GetInstance();
		void AddScene(Scene* newScene);
		void ChangeScene(const char* sceneName);
		void ChangeSceneWithoutInitialize(const char* sceneName);
		Scene* GetCurrentScene();
		Scene* GetBeforeScene();
		Scene* GetScene(const char* sceneName);

		void Initialize();
		void Update();
		void Draw();
		void LateDraw();
	private:
		SceneManager();
		SceneManager(const SceneManager&) = delete;
		~SceneManager();
		SceneManager& operator=(const SceneManager&) = delete;
	};
}
