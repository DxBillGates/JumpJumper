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
			Timer();
			virtual ~Timer();
			void SetIsShow(bool b);
			float GetElapsedTime();
			float GetElapsedApplicationTime();
			void Start();
			void End(bool isShow = true, const char* addComment = "");
			bool Update();
			void SetFrameRate(float value);
		};
	}
}
