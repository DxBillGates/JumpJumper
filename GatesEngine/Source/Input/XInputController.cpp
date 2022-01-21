#include "..\..\Header\Input\XInputController.h"
#include <math.h>
#pragma comment (lib, "xinput.lib")

int GatesEngine::XInputController::nextPadIndex = 0;

GatesEngine::XInputController::XInputController()
{
	ctrlState = {};
	mIndex = nextPadIndex;
	DWORD result = XInputGetState(mIndex, &ctrlState);

	// 各チェックボタンのセット
	for (int i = 0; i < 16; ++i)
	{
		int addValue = 0;
		if (i >= 10)addValue = 2;
		int padValue = (int)powf(2, i + addValue);
		oldKey[i].button = (XInputControllerButton)padValue;
		oldKey[i].flag = false;
	}

	// LT,RTだけ番号が2のべき乗じゃないのでセット
	oldKey[14].button = XInputControllerButton::XINPUT_LT;
	oldKey[15].button = XInputControllerButton::XINPUT_RT;

	// 次に使用されているパッドを認識する際に使うインデックスを進める
	++nextPadIndex;
}

GatesEngine::XInputController::~XInputController()
{
}

void GatesEngine::XInputController::Initialize()
{
	// 各チェックボタンのセット
	for (int i = 0; i < 16; ++i)
	{
		int padValue = (int)powf(2, i);
		oldKey[i].button = (XInputControllerButton)padValue;
		oldKey[i].flag = false;
	}

	// LT,RTだけ番号が2のべき乗じゃないのでセット
	oldKey[14].button = XInputControllerButton::XINPUT_LT;
	oldKey[15].button = XInputControllerButton::XINPUT_RT;
}

void GatesEngine::XInputController::Update()
{
	for (int i = 0; i < 14; ++i)
	{
		if (ctrlState.Gamepad.wButtons & (int)oldKey[i].button)
		{
			oldKey[i].flag = true;
		}
		else
		{
			oldKey[i].flag = false;
		}
	}
	if (ctrlState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)oldKey[14].flag = true;
	else oldKey[14].flag = false;
	if (ctrlState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)oldKey[15].flag = true;
	else oldKey[15].flag = false;
	//このフレーム内でのコントローラーの状態を取得
	DWORD result = XInputGetState(mIndex, &ctrlState);
}

bool GatesEngine::XInputController::CheckHitAllButton()
{
	for (auto& checkButton : oldKey)
	{
		if (checkButton.flag)
		{
			return true;
		}
	}
	return false;
}

bool GatesEngine::XInputController::CheckHitButton(XInputControllerButton button)
{
	for (auto& checkButton : oldKey)
	{
		if (checkButton.button == button)
		{
			return checkButton.flag;
		}
	}
	return false;
}

bool GatesEngine::XInputController::CheckHitButtonTrigger(XInputControllerButton button)
{
	// 2のべき乗で判定できないキーから判定
	if (oldKey[14].button == button && !oldKey[14].flag)
	{
		if (ctrlState.Gamepad.bLeftTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			oldKey[14].flag = true;
			return true;
		}
	}
	if (oldKey[15].button == button && !oldKey[15].flag)
	{
		if (ctrlState.Gamepad.bRightTrigger > XINPUT_GAMEPAD_TRIGGER_THRESHOLD)
		{
			oldKey[15].flag = true;
			return true;
		}
	}

	// 2のべき乗で設定されたキーの判定
	for (int i = 0; i < 14; ++i)
	{
		if (oldKey[i].button != button)continue;
		if (oldKey[i].flag)break;

		if (ctrlState.Gamepad.wButtons & (int)button)
		{
			oldKey[i].flag = true;
			return true;
		}
	}
	return false;
}

float GatesEngine::XInputController::GetRStickX()
{
	return ctrlState.Gamepad.sThumbRX / 32767.0f;
}

float GatesEngine::XInputController::GetRStickY()
{
	return ctrlState.Gamepad.sThumbRY / 32767.0f;
}

float GatesEngine::XInputController::GetLStickX()
{
	return ctrlState.Gamepad.sThumbLX / 32767.0f;
}

float GatesEngine::XInputController::GetLStickY()
{
	return ctrlState.Gamepad.sThumbLY / 32767.0f;
}

GatesEngine::Math::Vector2 GatesEngine::XInputController::GetRStick()
{
	return Math::Vector2(GetRStickX(), GetRStickY());
}

GatesEngine::Math::Vector2 GatesEngine::XInputController::GetLStick()
{
	return Math::Vector2(GetLStickX(), GetLStickY());
}
