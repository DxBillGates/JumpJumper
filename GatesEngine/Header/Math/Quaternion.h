#pragma once
#include "Vector3.h"
#include "Matrix4.h"
namespace GatesEngine
{
	namespace Math
	{
		struct Quaternion
		{
			float x;
			float y;
			float z;
			float w;

			Quaternion();
			Quaternion(float x, float y, float z, float w = 1.0f);
			Quaternion(const Vector3& v, float angle);
			Quaternion(const Matrix4x4& m);

			static Quaternion Normalize(const Quaternion& q);
			static Quaternion Conjugate(const Quaternion& q);
			static float Dot(const Quaternion& q1, const Quaternion& q2);
			static float Length(const Quaternion& q);
			static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t);
			static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t);
			static Matrix4x4 Rotation(const Quaternion& q);
			static Vector3 GetAxis(const Quaternion& q);

			Quaternion operator+();
			Quaternion operator-();
			Quaternion& operator+=(const Quaternion& q);
			Quaternion& operator-=(const Quaternion& q);
			Quaternion& operator*=(float s);
			Quaternion& operator/=(float s);
			Quaternion& operator*=(const Quaternion& q);
		};
		Quaternion operator+(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator-(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator*(const Quaternion& q1, const Quaternion& q2);
		Quaternion operator*(const Quaternion& q, float s);
		Quaternion operator*(float s, const Quaternion& q);
		Quaternion operator/(const Quaternion& q, float s);
	}
}