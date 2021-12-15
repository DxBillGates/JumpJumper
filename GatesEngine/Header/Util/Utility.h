#pragma once

namespace GatesEngine
{
	namespace Utility
	{
		/// <summary>
		/// Debug実行時に動作するprintf関数
		/// </summary>
		/// <param name="format">表示したい文字列</param>
		/// <param name="">書式指定子にあった変数</param>
		void Printf(const char* format, ...);
	}
}
