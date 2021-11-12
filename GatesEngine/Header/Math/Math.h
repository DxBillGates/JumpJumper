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
		/// 度数法から弧度法に変換する関数
		/// </summary>
		/// <param name="value">度数</param>
		/// <returns>ラジアン</returns>
		float ConvertToRadian(float value);

		/// <summary>
		/// 線形補間関数
		/// </summary>
		/// <param name="s">スタート</param>
		/// <param name="e">エンド</param>
		/// <param name="t">タイム</param>
		/// <returns>線形補間の値</returns>
		float Lerp(float s, float e, float t);
	}
}
