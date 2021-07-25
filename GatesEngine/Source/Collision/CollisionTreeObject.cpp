#include "..\..\Header\Collision\CollisionTreeObject.h"
#include "..\..\Header\Collision\CollisionTreeCell.h"

GatesEngine::CollisionTreeObject::CollisionTreeObject(Collider* pCollider)
	: collider(pCollider)
	, obj(nullptr)
	, nextObj(nullptr)
	, settingCell(nullptr)
{
}

GatesEngine::CollisionTreeObject::~CollisionTreeObject()
{
	Remove();
}

bool GatesEngine::CollisionTreeObject::Remove()
{
	if (!settingCell)return false;
	if(!settingCell->OnRemove(this))return false;

	if (obj)
	{
		obj->nextObj = nextObj;
	}
	if (nextObj)
	{
		nextObj->obj = obj;
	}

	obj = nextObj = nullptr;
	settingCell = nullptr;

	return true;
}

void GatesEngine::CollisionTreeObject::RegisterCell(CollisionTreeCell* cell)
{
	settingCell = cell;
}

GatesEngine::Collider* GatesEngine::CollisionTreeObject::GetCollider()
{
	return collider;
}

GatesEngine::CollisionTreeObject* GatesEngine::CollisionTreeObject::GetObject()
{
	return obj;
}

GatesEngine::CollisionTreeObject* GatesEngine::CollisionTreeObject::GetNextObject()
{
	return nextObj;
}

GatesEngine::CollisionTreeCell* GatesEngine::CollisionTreeObject::GetSettingCell()
{
	return settingCell;
}

void GatesEngine::CollisionTreeObject::SetCollider(Collider* pCollider)
{
	collider = pCollider;
}

void GatesEngine::CollisionTreeObject::SetObject(CollisionTreeObject* pObject)
{
	obj = pObject;
}

void GatesEngine::CollisionTreeObject::SetNextObject(CollisionTreeObject* pObject)
{
	nextObj = pObject;
}
