#pragma once
#include "..\Math\Math.h"
#include <vector>
#include <list>

namespace GatesEngine
{
	class Collider;
	class CollisionTreeCell;
	class CollisionTreeObject;
	class CollisionTreeManager
	{
	private:
		const int MAX_LEVEL = 7;
		Math::Vector3 minRange;
		Math::Vector3 maxRange;
		Math::Vector3 range;
		Math::Vector3 minAreaRange;
		int cellCount;
		int minLevel;
		std::vector<CollisionTreeCell*> cells;
		std::vector<int> pows;
	private:
		int GetMortonNumber(const Math::Vector3& min, const Math::Vector3& max);
		int BitSeparete3D(int n);
		int Get3DMortonNumber(int x, int y, int z);
		int GetPointElement(const Math::Vector3& pos);
		void CreateNewCell(int registerArea);
		bool GetCollisionList(int elem, std::vector<Collider*>& colliders, std::list<Collider*>& colliderStacks);
	public:
		CollisionTreeManager();
		~CollisionTreeManager();
		void Initialize(int level,const Math::Vector3& min,const Math::Vector3& max);
		bool Register(Collider* registerCollider,CollisionTreeObject* obj);
		int GetAllCollisionListSize(std::vector<Collider*>& colliders);
	};
}
