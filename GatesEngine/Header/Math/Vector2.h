#pragma once
namespace GatesEngine
{
	namespace Math
	{
		struct Vector2
		{
			float x;
			float y;

			Vector2();
			Vector2(float xy);
			Vector2(float x, float y);

			float Length();
			Vector2 Normalize();

			static float Distance(const Vector2& v1, const Vector2& v2);
			static Vector2 Normalize(const Vector2& v);
			static float Cross(const Vector2& v1, const Vector2& v2);
			static float Dot(const Vector2& v1, const Vector2& v2);
			static Vector2 Lerp(const Vector2& s, const Vector2& e, float t);

			Vector2 operator+();
			Vector2 operator-();
			Vector2& operator+=(const Vector2& v);
			Vector2& operator-=(const Vector2& v);
			Vector2& operator*=(float s);
			Vector2& operator/=(float s);
		};
		const Vector2 operator+(const Vector2& v1, const Vector2& v2);
		const Vector2 operator-(const Vector2& v1, const Vector2& v2);
		const Vector2 operator*(const Vector2& v, float s);
		const Vector2 operator*(float s, const Vector2& v);
		const Vector2 operator/(const Vector2& v, float s);
		const Vector2 operator/(float s, const Vector2& v);
	}
}