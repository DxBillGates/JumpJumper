#include "..\..\Header\Input\Mouse.h"
#include <stdio.h>

using Vector2 = GatesEngine::Math::Vector2;

GatesEngine::Mouse::Mouse()
{
}

GatesEngine::Mouse::~Mouse()
{
}

void GatesEngine::Mouse::Create(IDirectInput8 * input, HWND hwnd)
{
	HRESULT result;
	result = input->CreateDevice(GUID_SysMouse, &device, NULL);
	result = device->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	result = device->SetDataFormat(&c_dfDIMouse2);
}

void GatesEngine::Mouse::Initialize()
{
	mousePos = Vector2();
	mouseMove = Vector2();
}

void GatesEngine::Mouse::Update()
{

	beforeMouseState = currentMouseState;

	HRESULT result;

	result = device->GetDeviceState(sizeof(currentMouseState), &currentMouseState);

	if (result != S_OK) {
		device->Acquire();
		result = device->GetDeviceState(sizeof(currentMouseState), &currentMouseState);
	}
	POINT mousePoint{};
	GetCursorPos(&mousePoint);
	mouseMove.x = mousePos.x - mousePoint.x;
	mouseMove.y = mousePos.y - mousePoint.y;

	mousePos.x = (float)mousePoint.x;
	mousePos.y = (float)mousePoint.y;
}

bool GatesEngine::Mouse::GetCheckHitButtonAll()
{
	for (int i = 0; i < 3; i++)
	{
		if (currentMouseState.rgbButtons[i])return true;
	}
	return false;
}

bool GatesEngine::Mouse::GetCheckHitButton(MouseButtons button)
{
	return currentMouseState.rgbButtons[(int)button];
}

bool GatesEngine::Mouse::GetCheckPressTrigger(MouseButtons button)
{
	return (!beforeMouseState.rgbButtons[(int)button]) && currentMouseState.rgbButtons[(int)button];
}

bool GatesEngine::Mouse::GetCheckReleaseTrigger(MouseButtons button)
{
	return (beforeMouseState.rgbButtons[(int)button]) && !currentMouseState.rgbButtons[(int)button];
}

float GatesEngine::Mouse::GetWheelValue()
{
	return (float)currentMouseState.lZ;
}

Vector2 GatesEngine::Mouse::GetMouseMove()
{
	return mouseMove;
}

Vector2 GatesEngine::Mouse::GetMousePos()
{
	return mousePos;
}

void GatesEngine::Mouse::SetMouseCursor(const Vector2& setPos)
{
	SetCursorPos((int)setPos.x, (int)setPos.y);
	mousePos = setPos;
}
