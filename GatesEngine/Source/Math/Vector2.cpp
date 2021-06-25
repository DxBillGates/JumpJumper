#include "..\..\Header\Math\Vector2.h"
#include <math.h>
GatesEngine::Math::Vector2::Vector2() :x(0), y(0)
{
}

GatesEngine::Math::Vector2::Vector2(float xy) : x(xy), y(xy)
{
}

GatesEngine::Math::Vector2::Vector2(float x, float y) : x(x), y(y)
{
}

float GatesEngine::Math::Vector2::Length()
{
	return sqrtf(x * x + y * y);
}

GatesEngine::Math::Vector2 GatesEngine::Math::Vector2::Normalize()
{
	float length = Length();
	if (length == 0)
	{
		return Vector2();
	}
	return *this / length;
}

float GatesEngine::Math::Vector2::Distance(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 v = v1 - v2;
	return v.Length();
}

GatesEngine::Math::Vector2 GatesEngine::Math::Vector2::Normalize(const Vector2 & v)
{
	Vector2 v1 = v;
	float length = v1.Length();
	if (length == 0)
	{
		return Vector2();
	}
	return v1 / length;
}

float GatesEngine::Math::Vector2::Cross(const Vector2 & v1, const Vector2 & v2)
{
	return v1.x * v2.y - v1.y * v2.x;
}

float GatesEngine::Math::Vector2::Dot(const Vector2 & v1, const Vector2 & v2)
{
	return v1.x * v2.x + v1.y * v2.y;
}

GatesEngine::Math::Vector2 GatesEngine::Math::Vector2::Lerp(const Vector2 & s, const Vector2 & e, float t)
{
	return s * (1.0f - t) + e * t;
}

GatesEngine::Math::Vector2 GatesEngine::Math::Vector2::operator+()
{
	return *this;
}

GatesEngine::Math::Vector2 GatesEngine::Math::Vector2::operator-()
{
	return Vector2(-x, -y);
}

GatesEngine::Math::Vector2 & GatesEngine::Math::Vector2::operator+=(const Vector2 & v)
{
	x += v.x;
	y += v.y;
	return *this;
}

GatesEngine::Math::Vector2 & GatesEngine::Math::Vector2::operator-=(const Vector2 & v)
{
	x -= v.x;
	y -= v.y;
	return *this;
}

GatesEngine::Math::Vector2 & GatesEngine::Math::Vector2::operator*=(float s)
{
	x *= s;
	y *= s;
	return *this;
}

GatesEngine::Math::Vector2 & GatesEngine::Math::Vector2::operator/=(float s)
{
	x /= s;
	y /= s;
	return *this;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator+(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a += v2;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator-(const Vector2 & v1, const Vector2 & v2)
{
	Vector2 a(v1);
	return a -= v2;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator*(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a *= s;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator*(float s, const Vector2 & v)
{
	return v * s;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator/(const Vector2 & v, float s)
{
	Vector2 a(v);
	return a /= s;
}

const GatesEngine::Math::Vector2 GatesEngine::Math::operator/(float s, const Vector2 & v)
{
	return v / s;
}