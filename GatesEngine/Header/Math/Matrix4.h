#pragma once
#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Axis.h"
namespace GatesEngine
{
	namespace Math
	{
		//4*4çsóÒ
		struct Matrix4x4
		{
			union
			{
				struct
				{
					float _11, _12, _13, _14;
					float _21, _22, _23, _24;
					float _31, _32, _33, _34;
					float _41, _42, _43, _44;
				};
				float m[4][4];
			};

			Matrix4x4();
			Matrix4x4
			(
				float m00, float m01, float m02, float m03,
				float m10, float m11, float m12, float m13,
				float m20, float m21, float m22, float m23,
				float m30, float m31, float m32, float m33
			);

			static Matrix4x4 Identity();
			static Axis GetAxis(const Matrix4x4& m);
			static Matrix4x4 Scale(const Vector3& v);
			static Matrix4x4 RotationX(float angle);
			static Matrix4x4 RotationY(float angle);
			static Matrix4x4 RotationZ(float angle);
			static Matrix4x4 RotationZXY(const Vector3& angles);
			static Matrix4x4 Translate(const Vector3& v);
			static Vector3 Transform(const Vector3& v, const Matrix4x4& m);
			static Matrix4x4 Transpose(const Matrix4x4& m);
			static Matrix4x4 Inverse(const Matrix4x4& m);
			static float Determinant(const Matrix4x4& m);

			const Axis& GetAxis() const;
			static Matrix4x4 GetViewMatrixLookAt(const Vector3& pos, const Vector3& focasPos, const Vector3& upDir);
			static Matrix4x4 GetViewMatrixLookTo(const Vector3& pos, const Vector3& direction, const Vector3& upDir);
			static Matrix4x4 GetPerspectiveMatrix(float fov, float aspect, float nearClip = 1.0f, float farClip = 10000.0f);
			static Matrix4x4 GetOrthographMatrix(const Vector2& size);
			static Matrix4x4 GetOrthographMatrix(const Vector2& size, float nearClip, float farClip);
		};
		Matrix4x4& operator*=(Matrix4x4& m1, const Matrix4x4& m2);
		Matrix4x4 operator*(const Matrix4x4& m1, const Matrix4x4& m2);
		Vector3 operator*(const Vector3& v, const Matrix4x4& m);
	}
}
