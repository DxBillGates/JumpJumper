#pragma once
#include "Math.h"

namespace GatesEngine
{
	struct Transform
	{
		Math::Vector3 position = {};
		Math::Vector3 rotation = {};
		Math::Vector3 scale = {1,1,1};

		Math::Matrix4x4 GetMatrix();
		Math::Vector3 GetEulerAngle();
	};
}