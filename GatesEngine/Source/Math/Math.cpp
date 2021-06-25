#include "..\..\Header\Math\Math.h"

float GatesEngine::Math::ConvertToRadian(float value)
{
	if (value == 0)return 0;
	float result = PI / 180.0f * value;
	return result;
}
