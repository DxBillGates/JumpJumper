#pragma once
#include "CollisionTreeManager.h"
#include "..\..\Header\Component\Collider.h"

namespace GatesEngine
{
	class CollisionManager
	{
	private:
		std::vector<Collider*> colliderComponents;
		CollisionTreeManager* collisionTreeManager;
	public:
		CollisionManager();
		~CollisionManager();
		Collider* AddColliderComponent(Collider* collider);
		void AddRegisterTree(Collider* collider);
		void UpdateTree(Collider* collider, CollisionTreeObject* object);
		void Update();
		void CollisionCheck();
		static bool CheckAABB(Collider* collider1,Collider* collider2);
		static bool CheckSphereToSphere(Collider* collider1, Collider* collider2);
		void Initialize(int level, const Math::Vector3& min, const Math::Vector3& max);
	};
}
