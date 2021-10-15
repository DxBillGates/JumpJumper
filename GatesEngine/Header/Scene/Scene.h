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
		Scene();
		Scene(const char* sceneName);
		Scene(const char* sceneName, Application* app);
		virtual ~Scene();
		virtual void Initialize() = 0;
		virtual void Update() = 0;
		virtual void Draw() = 0;
		virtual void LateDraw() {}
		const char* GetSceneName();
		bool IsChangeScene();
		GameObjectManager* GetGameObjectManager();
	};
}
