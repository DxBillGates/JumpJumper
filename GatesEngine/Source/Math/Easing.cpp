#include "..\..\Header\Math\Math.h"
#include "..\..\Header\Math\Easing.h"
#include <cmath>

float GatesEngine::Math::Easing::Lerp(float s, float e, float t)
{
	return s * (1.0f - t) + e * t;;
}

float GatesEngine::Math::Easing::EaseInSine(float t)
{
	return 1 - cosf((t * PI) / 2);
}

float GatesEngine::Math::Easing::EaseOutSine(float t)
{
	return sinf((t * PI) / 2);
}

float GatesEngine::Math::Easing::EaseInOutSine(float t)
{
	return -(cosf(PI * t) - 1) / 2;
}

float GatesEngine::Math::Easing::EaseInQuad(float t)
{
	return t * t;
}

float GatesEngine::Math::Easing::EaseOutQuad(float x)
{
	return 1 - (1 - x) * (1 - x);
}

float GatesEngine::Math::Easing::EaseInOutQuad(float x)
{
	return x < 0.5f ? 2 * x * x : 1 - powf(-2 * x + 2, 2) / 2;
}

float GatesEngine::Math::Easing::EaseInCubic(float x)
{
	return x * x * x;
}

float GatesEngine::Math::Easing::EaseOutCubic(float x)
{
	return 1 - powf(1 - x, 3);
}

float GatesEngine::Math::Easing::EaseInOutCubic(float x)
{
	return x < 0.5f ? 4 * x * x * x : 1 - powf(-2 * x + 2, 3) / 2;
}

float GatesEngine::Math::Easing::EaseInQuart(float x)
{
	return x * x * x * x;
}

float GatesEngine::Math::Easing::EaseOutQuart(float x)
{
	return 1 - powf(1 - x, 4);
}

float GatesEngine::Math::Easing::EaseInOutQuart(float x)
{
	return x < 0.5f ? 8 * x * x * x * x : 1 - powf(-2 * x + 2, 4) / 2;
}

float GatesEngine::Math::Easing::EaseInQuint(float x)
{
	return x * x * x * x * x;
}

float GatesEngine::Math::Easing::EaseOutQuint(float x)
{
	return 1 - powf(1 - x, 5);
}

float GatesEngine::Math::Easing::EaseInOutQuint(float x)
{
	return x < 0.5f ? 16 * x * x * x * x * x : 1 - powf(-2 * x + 2, 5) / 2;
}

float GatesEngine::Math::Easing::EaseInExpo(float x)
{
	return x == 0 ? 0 : powf(2, 10 * x - 10);
}

float GatesEngine::Math::Easing::EaseOutExpo(float x)
{
	return x == 1 ? 1 : 1 - powf(2, -10 * x);
}

float GatesEngine::Math::Easing::EaseInOutExpo(float x)
{
	return x == 0 ? 0 : x == 1 ? 1 : x < 0.5f ? powf(2, 20 * x - 10) / 2 : (2 - powf(2, -20 * x + 10)) / 2;
}

float GatesEngine::Math::Easing::EaseInCirc(float x)
{
	return 1 - sqrtf(1 - powf(x, 2));
}

float GatesEngine::Math::Easing::EaseOutCirc(float x)
{
	return sqrtf(1 - powf(x - 1, 2));
}

float GatesEngine::Math::Easing::EaseInOutCirc(float x)
{
	return x < 0.5f ? (1 - sqrtf(1 - powf(2 * x, 2))) / 2 : (sqrtf(1 - powf(-2 * x + 2, 2)) + 1) / 2;
}

float GatesEngine::Math::Easing::EaseInBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return c3 * x * x * x - c1 * x * x;
}

float GatesEngine::Math::Easing::EaseOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c3 = c1 + 1;

	return 1 + c3 * powf(x - 1, 3) + c1 * powf(x - 1, 2);
}

float GatesEngine::Math::Easing::EaseInOutBack(float x)
{
	const float c1 = 1.70158f;
	const float c2 = c1 * 1.525f;

	return x < 0.5 ? (powf(2 * x, 2) * ((c2 + 1) * 2 * x - c2)) / 2 : (powf(2 * x - 2, 2) * ((c2 + 1) * (x * 2 - 2) + c2) + 2) / 2;
}

float GatesEngine::Math::Easing::EaseInElastic(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0 ? 0 : x == 1 ? 1 : -powf(2, 10 * x - 10) * sinf((x * 10 - 10.75f) * c4);
}

float GatesEngine::Math::Easing::EaseOutElastic(float x)
{
	const float c4 = (2 * PI) / 3;

	return x == 0
		? 0
		: x == 1
		? 1
		: powf(2, -10 * x) * sinf((x * 10 - 0.75f) * c4) + 1;
}

float GatesEngine::Math::Easing::EaseInOutElastic(float x)
{
	const float c5 = (2 * PI) / 4.5f;

	return x == 0
		? 0
		: x == 1
		? 1
		: x < 0.5
		? -(powf(2, 20 * x - 10) * sinf((20 * x - 11.125f) * c5)) / 2
		: (powf(2, -20 * x + 10) * sinf((20 * x - 11.125f) * c5)) / 2 + 1;
}

float GatesEngine::Math::Easing::EaseInBounce(float x)
{
	return 1 - EaseOutBounce(1 - x);
}

float GatesEngine::Math::Easing::EaseOutBounce(float x)
{
	const float n1 = 7.5625f;
	const float d1 = 2.75f;

	if (x < 1 / d1) {
		return n1 * x * x;
	}
	else if (x < 2 / d1) {
		return n1 * (x -= 1.5f / d1) * x + 0.75f;
	}
	else if (x < 2.5f / d1) {
		return n1 * (x -= 2.25f / d1) * x + 0.9375f;
	}
	else {
		return n1 * (x -= 2.625f / d1) * x + 0.984375f;
	}
}

float GatesEngine::Math::Easing::EaseInOutBounce(float x)
{
	return x < 0.5f
		? (1 - EaseOutBounce(1 - 2 * x)) / 2
		: (1 + EaseOutBounce(2 * x - 1)) / 2;
}
