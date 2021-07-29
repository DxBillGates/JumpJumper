#include "..\..\Header\Component\Collider.h"

GatesEngine::Collider::Collider()
	: type(ColliderType::CUBE)
	, size({ 1 })
	, treeObj(nullptr)
{
}

GatesEngine::Collider::~Collider()
{
	delete treeObj;
}

void GatesEngine::Collider::SetType(ColliderType sType)
{
	type = sType;
}

GatesEngine::ColliderType GatesEngine::Collider::GetType()
{
	return type;
}

void GatesEngine::Collider::SetSize(const Math::Vector3& sSize)
{
	size = sSize;
}

GatesEngine::Math::Vector3 GatesEngine::Collider::GetSize()
{
	return size;
}

void GatesEngine::Collider::SetCollisionTreeObject(CollisionTreeObject* obj)
{
	treeObj = obj;
}

GatesEngine::CollisionTreeObject* GatesEngine::Collider::GetCollisionTreeObject()
{
	return treeObj;
}
