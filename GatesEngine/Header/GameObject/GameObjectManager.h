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
		void Start();
		void Update();
		void Draw();
	};
}
