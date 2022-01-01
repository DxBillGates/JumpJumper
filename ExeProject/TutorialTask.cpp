#include "TutorialTask.h"
#include "Header/Util/Utility.h"

void TutorialTask::ErrorTaskCheck()
{
	if (errorTaskFlag)
	{
		errorTaskFlag = false;
		GatesEngine::Utility::Printf("���炩�̎���ɂ��^�X�N�����s�ł��܂���ł����B\n");
	}
}

void TutorialTask::IncrementClearTaskCount()
{
	++clearTaskCount;
}

TutorialTask::TutorialTask()
	: preClearFlag(false)
	, clearFlag(false)
	, errorTaskFlag(false)
{
}

TutorialTask::~TutorialTask()
{
}

void TutorialTask::Initialize()
{
	preClearFlag = clearFlag = errorTaskFlag = false;
}

void TutorialTask::Update()
{
}

bool TutorialTask::GetClearFlag()
{
	return (clearFlag);
}

bool TutorialTask::IsClearTask()
{
	bool flag = (clearTaskCount >= 3);
	return flag;
}
