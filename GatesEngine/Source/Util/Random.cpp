#include "..\..\Header\Util\Random.h"
#include <random>

float GatesEngine::Random::Rand(float minValue, float maxValue)
{
	return ((float)std::rand() / RAND_MAX * maxValue) - (-minValue) / 2;
}
