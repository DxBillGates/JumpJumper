#pragma once
#include "..\Math\Vector3.h"
#include <vector>

namespace GatesEngine
{
	class CollisionTreeObject;
	class CollisionTreeManager;
	class Collider;
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
		static bool CheckAABB(Collider* collider1, Collider* collider2);
		static bool CheckSphereToSphere(Collider* collider1, Collider* collider2);
		static bool CheckAABBToRay(Collider* collider,const Math::Vector3& linePos, const Math::Vector3& lineDir);
		void Initialize(int level, const Math::Vector3& min, const Math::Vector3& max);
	};
}
