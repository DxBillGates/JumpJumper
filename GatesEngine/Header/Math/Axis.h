#pragma once
#include "Vector3.h"
namespace GatesEngine
{
	namespace Math
	{
		struct Axis
		{
			Vector3 x = {1,0,0};
			Vector3 y = {0,1,0};
			Vector3 z = {0,0,1};
		};
	}
}