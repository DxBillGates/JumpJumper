#pragma once
#include "GameObject.h"
namespace GatesEngine
{
	class GameObjectManager
	{
	private:
		std::vector<GameObject*> gameObjects;
	public:
		GameObjectManager();
		~GameObjectManager();
		GameObject* Add(GameObject* newGameObject);
		GameObject* Find(const char* name, const char* tag = nullptr);
		void Start();
		void Update();
		void Draw();
	};
}
