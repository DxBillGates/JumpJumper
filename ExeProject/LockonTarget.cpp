#include "LockonTarget.h"

LockonTarget::LockonTarget()
	: target(nullptr)
	, lockonTime(0)
	, MAX_LOCKON_TIME(5)
	, isLockon(false)
{
}

LockonTarget::~LockonTarget()
{
}

void LockonTarget::Initialize()
{
	target = nullptr;
	lockonTime = 0;
	isLockon = false;
}

void LockonTarget::Update()
{
	if (!target)return;
	if (lockonTime >= MAX_LOCKON_TIME)Initialize();

	const float PER_FRAME = 1.0f / 60.0f;
	lockonTime += PER_FRAME;
}

void LockonTarget::SetLockonTarget(GatesEngine::GameObject* target)
{
	if (!target)return;
	this->target = target;
	isLockon = true;
}

GatesEngine::GameObject* LockonTarget::GetTarget()
{
	return target;
}

float LockonTarget::GetLockTime()
{
	return lockonTime;
}

bool LockonTarget::GetIsLockon()
{
	return isLockon;
}

const int LockonTarget::GetMaxLockonTime()
{
	return MAX_LOCKON_TIME;
}
