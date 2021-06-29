#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix4.h"
#include "Quaternion.h"
#include "Easing.h"

namespace GatesEngine
{
	namespace Math
	{
		const float PI = 3.14159265f;

		float ConvertToRadian(float value);
		float Lerp(float s, float e, float t);
	}
}
