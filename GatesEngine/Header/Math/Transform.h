#pragma once
#include "Math.h"

namespace GatesEngine
{
	struct Transform
	{
		Math::Vector3 position = {};
		Math::Vector3 rotation = {};
		Math::Vector3 scale = { 1,1,1 };

		Transform()
		{
		}

		Transform(const Math::Vector3& pos, const Math::Vector3& rotation, const Math::Vector3& scale)
			: position(pos)
			, rotation(rotation)
			, scale(scale)
		{
		}

		Transform(const Transform& transform)
			: position(transform.position)
			, rotation(transform.rotation)
			, scale(transform.scale)
		{
		}
		Math::Matrix4x4 GetMatrix();
		Math::Vector3 GetEulerAngle();
		Math::Vector3 GetForward();
		Math::Vector3 GetRight();
		Math::Vector3 GetUp();
	};
}