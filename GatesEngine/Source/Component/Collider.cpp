#include "..\..\Header\Component\Collider.h"

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
