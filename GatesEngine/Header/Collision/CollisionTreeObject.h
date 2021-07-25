#pragma once
#include "..\..\Header\Component\Collider.h"

namespace GatesEngine
{
	class CollisionTreeCell;
	class CollisionTreeObject
	{
	private:
		Collider* collider;
		CollisionTreeObject* obj;
		CollisionTreeObject* nextObj;
		CollisionTreeCell* settingCell;
	public:
		CollisionTreeObject(Collider* pCollider);
		~CollisionTreeObject();
		bool Remove();
		void RegisterCell(CollisionTreeCell* cell);
		Collider* GetCollider();
		CollisionTreeObject* GetObject();
		CollisionTreeObject* GetNextObject();
		CollisionTreeCell* GetSettingCell();
		void SetCollider(Collider* pCollider);
		void SetObject(CollisionTreeObject* pObject);
		void SetNextObject(CollisionTreeObject* pObject);
	};
}
