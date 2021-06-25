#pragma once
#include "Component.h"
#include "..\Math\Math.h"

namespace GatesEngine
{
	enum class ColliderType
	{
		CUBE,
		SPHERE,
	};
	class Collider : public Component
	{
	private:
		ColliderType type;
		Math::Vector3 size;
	public:
		void SetType(ColliderType sType);
		ColliderType GetType();
		void SetSize(const Math::Vector3& sSize);
		Math::Vector3 GetSize();
	};
}
