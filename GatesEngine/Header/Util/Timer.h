#pragma once
#include <Windows.h>
namespace GatesEngine
{
	namespace Util
	{
		class Timer
		{
		private:
			bool isShow;
			double fps;
			double frameTime;
			LARGE_INTEGER timeStart;
			LARGE_INTEGER timeEnd;
			LARGE_INTEGER timeFreq;
			LARGE_INTEGER startBuffer;
			LARGE_INTEGER endBuffer;
			float setFrameRate;
			bool isSetFrameRate;
			float elapsedApplicationTime;
		public:
			/// <summary>
			/// コンストラクタ
			/// </summary>
			Timer();

			/// <summary>
			/// 仮想デストラクタ
			/// </summary>
			virtual ~Timer();

			/// <summary>
			/// FPSをデバッグウィンドウに表示させるか設定する関数
			/// </summary>
			/// <param name="b">成否</param>
			void SetIsShow(bool b);

			/// <summary>
			/// 前フレームに使用された時間をmsで返す関数
			/// </summary>
			/// <returns>前フレームに使用された時間(ms)</returns>
			float GetElapsedTime();

			/// <summary>
			/// アプリケーションが始まってからの時間(s)を返す関数
			/// </summary>
			/// <returns>アプリケーションが始まってからの時間(s)</returns>
			float GetElapsedApplicationTime();

			/// <summary>
			/// フレームの時間を図るためのスタート関数
			/// </summary>
			void Start();

			/// <summary>
			/// フレームの時間を図るためのエンド関数
			/// </summary>
			/// <param name="isShow">msを表示するか</param>
			/// <param name="addComment">追加コメント</param>
			void End(bool isShow = true, const char* addComment = "");

			/// <summary>
			/// 指定されているFPS以上出ないようにする関数
			/// </summary>
			/// <returns>成否</returns>
			bool Update();

			/// <summary>
			/// アプリケーションの最大フレームレートを設定する関数
			/// </summary>
			/// <param name="value"></param>
			void SetFrameRate(float value);
		};
	}
}
