#include "..\..\Header\Util\Timer.h"
#include <stdio.h>
#include <string>
#pragma comment(lib,"winmm.lib")

GatesEngine::Util::Timer::Timer()
	:isShow(true)
	, fps(0)
	, frameTime(0)
	, timeStart({})
	, timeEnd({})
	, timeFreq({})
	, startBuffer({})
	, endBuffer({})
	, setFrameRate(0)
	, isSetFrameRate(false)
	, elapsedApplicationTime(0)
{
	QueryPerformanceFrequency(&timeFreq);
	QueryPerformanceCounter(&timeStart);
}

GatesEngine::Util::Timer::~Timer()
{
}

void GatesEngine::Util::Timer::SetIsShow(bool b)
{
	isShow = b;
}

float GatesEngine::Util::Timer::GetElapsedTime()
{
	return (float)frameTime;
}

float GatesEngine::Util::Timer::GetElapsedApplicationTime()
{
	return elapsedApplicationTime;
}

void GatesEngine::Util::Timer::Start()
{
	QueryPerformanceCounter(&startBuffer);
}

void GatesEngine::Util::Timer::End(bool isShow, const char* addComment)
{
	QueryPerformanceCounter(&endBuffer);
	double elapsedTime = static_cast<double>(endBuffer.QuadPart - startBuffer.QuadPart) / static_cast<double>(timeFreq.QuadPart);
	float fps = 1.0f / (float)elapsedTime;
	if (isShow)printf("%3.5fms : %3.0f :%s\n", (float)elapsedTime * 1000, fps, addComment);
}

bool GatesEngine::Util::Timer::Update()
{
	if (!isSetFrameRate)return false;
	if (setFrameRate <= 0)return false;

	const float SET_FRAME_TIME = 1.0f / setFrameRate;
	QueryPerformanceCounter(&timeEnd);
	frameTime = static_cast<double>(timeEnd.QuadPart - timeStart.QuadPart) / static_cast<double>(timeFreq.QuadPart);
	if (frameTime < SET_FRAME_TIME)
	{
		//Sleep‚ÌŽžŠÔ‚ðŒvŽZ
		DWORD sleepTime = static_cast<DWORD>((SET_FRAME_TIME - frameTime) * 1000);
		timeBeginPeriod(1);
		Sleep(sleepTime);
		timeEndPeriod(1);
		return true;
	}
	timeStart = timeEnd;
	fps = 1.0f / frameTime;
	elapsedApplicationTime += (float)frameTime;
#ifdef _DEBUG
	if (isShow)printf("%3.3ffps\n", fps);
#else
	char numStr[16];
	sprintf_s(numStr, "fps:%d\n", (int)fps);
	OutputDebugString(numStr);
	//if (isShow)printf("%3.3ffps\n", fps);
#endif

	return false;
}

void GatesEngine::Util::Timer::SetFrameRate(float value)
{
	setFrameRate = value;
	isSetFrameRate = true;
}
