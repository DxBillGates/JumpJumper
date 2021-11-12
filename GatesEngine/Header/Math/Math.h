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

		/// <summary>
		/// �x���@����ʓx�@�ɕϊ�����֐�
		/// </summary>
		/// <param name="value">�x��</param>
		/// <returns>���W�A��</returns>
		float ConvertToRadian(float value);

		/// <summary>
		/// ���`��Ԋ֐�
		/// </summary>
		/// <param name="s">�X�^�[�g</param>
		/// <param name="e">�G���h</param>
		/// <param name="t">�^�C��</param>
		/// <returns>���`��Ԃ̒l</returns>
		float Lerp(float s, float e, float t);
	}
}
