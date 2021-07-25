#pragma once

namespace GatesEngine
{
	class CollisionTreeObject;
	class CollisionTreeCell
	{
	private:
		CollisionTreeObject* latestTreeObj;
	public:
		CollisionTreeCell();
		~CollisionTreeCell();
		bool PushObject(CollisionTreeObject* pushObj);
		bool OnRemove(CollisionTreeObject* removeobj);
		CollisionTreeObject* GetFirstObject();
	};
}
