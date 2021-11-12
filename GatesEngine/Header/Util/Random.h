#pragma once
namespace GatesEngine
{
	namespace Random
	{
		/// <summary>
		/// 範囲を指定してランダムな値を返す関数
		/// </summary>
		/// <param name="minValue">範囲</param>
		/// <param name="maxValue">範囲</param>
		/// <returns>ランダムな値</returns>
		float Rand(float minValue, float maxValue);
	}
}