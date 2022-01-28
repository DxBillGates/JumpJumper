#include "FlagController.h"

FlagController::FlagController()
{
}

FlagController::~FlagController()
{
}

void FlagController::Initialize()
{

	flag = false;
	preElapsedTime = 0;
	elapsedTime = 0;
	PROPERTY_MAX_TIME = 1;
}

void FlagController::Update(float deltaTime)
{
	if (!flag)return;

	preElapsedTime = elapsedTime;
	float addValue = deltaTime / PROPERTY_MAX_TIME;
	elapsedTime += addValue;
}

bool FlagController::GetOverTimeTrigger()
{
	if (preElapsedTime < 1 && elapsedTime >= 1)
	{
		return true;
	}
	return false;
}

bool FlagController::GetFlag()
{
	return flag;
}

float FlagController::GetTime()
{
	return elapsedTime;
}

void FlagController::SetFlag(bool setFlag)
{
	flag = setFlag;
}

void FlagController::SetTime(float setValue)
{
	elapsedTime = setValue;
}

void FlagController::SetMaxTimeProperty(const float value)
{
	PROPERTY_MAX_TIME = value;
}

bool FlagController::IsOverTime(float value)
{
	return (elapsedTime >= value);
}

bool FlagController::IsUnderTime(float value)
{
	return (elapsedTime <= value);
}
