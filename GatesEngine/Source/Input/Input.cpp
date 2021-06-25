#include "..\..\Header\Input\Input.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"Xinput.lib")

GatesEngine::Input * GatesEngine::Input::GetInstance()
{
	static Input instance;
	return &instance;
}

void GatesEngine::Input::Create(HWND hwnd, HINSTANCE hInstance)
{
	if (dInput != nullptr)return;
	HRESULT result;
	result = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&dInput, nullptr);
	keyboard.Create(dInput,hwnd);
	mouse.Create(dInput, hwnd);
}

void GatesEngine::Input::Initialize()
{
	keyboard.Initialize();
	mouse.Initialize();
}

void GatesEngine::Input::Update()
{
	keyboard.Update();
	mouse.Update();
}

GatesEngine::Keyboard * GatesEngine::Input::GetKeyboard()
{
	return &keyboard;
}

GatesEngine::Mouse * GatesEngine::Input::GetMouse()
{
	return &mouse;
}

GatesEngine::Input::Input()
	:dInput(nullptr)
	,keyboard(Keyboard())
	,mouse(Mouse())
{
}
