#include "..\..\Header\Input\Keyboard.h"

GatesEngine::Keyboard::Keyboard()
{
	memset(&currentKeys, 0, sizeof(currentKeys));
	memset(&beforeKeys, 0, sizeof(beforeKeys));
}

GatesEngine::Keyboard::~Keyboard()
{
	device->Release();
}

void GatesEngine::Keyboard::Create(IDirectInput8 * input, HWND hwnd)
{
	HRESULT result;
	result = input->CreateDevice(GUID_SysKeyboard, &device, NULL);
	result = device->SetDataFormat(&c_dfDIKeyboard);
	result = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE/* | DISCL_NOWINKEY*/);
}

void GatesEngine::Keyboard::Initialize()
{
	memset(&currentKeys, 0, sizeof(currentKeys));
	memset(&beforeKeys, 0, sizeof(beforeKeys));
}

void GatesEngine::Keyboard::Update()
{
	device->Acquire();
	for (int i = 0; i < 256; ++i)
	{
		beforeKeys[i] = currentKeys[i];
	}
	device->GetDeviceState(sizeof(currentKeys), &currentKeys);
}

bool GatesEngine::Keyboard::CheckPressTrigger(Keys key)
{
	bool flag = false;
	int index = (int)key;
	if (!(beforeKeys[index] & 0x80) && (currentKeys[index] & 0x80))
	{
		flag = true;
	}
	return flag;
}

bool GatesEngine::Keyboard::CheckReleaseTrigger(Keys key)
{
	bool flag = false;
	int index = (int)key;
	if ((beforeKeys[index] & 0x80) && !(currentKeys[index] & 0x80))
	{
		flag = true;
	}
	return flag;
}

bool GatesEngine::Keyboard::CheckHitKey(Keys key)
{
	bool flag = false;
	int index = (int)key;
	if ((currentKeys[index] & 0x80))
	{
		flag = true;
	}
	return flag;
}

bool GatesEngine::Keyboard::CheckHitKeyAll()
{
	bool flag = false;
	for (int i = 0; i < 256; ++i)
	{
		if (currentKeys[i])
		{
			flag = true;
			break;
		}
	}
	return flag;
}
