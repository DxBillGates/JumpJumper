#include "..\..\Header\Input\Mouse.h"
#include <stdio.h>

using Vector2 = GatesEngine::Math::Vector2;

GatesEngine::Mouse::Mouse()
	: beforeMouseState({})
	, currentMouseState({})
	, device(nullptr)
	, hwnd()
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
	this->hwnd = hwnd;
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
	//ScreenToClient(hwnd, &mousePoint);
	float a = (mousePoint.y / 1080.0f) * 23 * 3;
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
	//return mouseMove;
	return { (float)-currentMouseState.lX,(float)-currentMouseState.lY };
}

Vector2 GatesEngine::Mouse::GetMousePos()
{
	POINT point = { (LONG)mousePos.x,(LONG)mousePos.y };
	ScreenToClient(hwnd, &point);
	float a = (point.y / 1080.0f) * 23 * 3;
	return {(float)point.x,(float)point.y + a};
}

void GatesEngine::Mouse::SetMouseCursor(const Vector2& setPos)
{
	POINT point = { (LONG)setPos.x,(LONG)setPos.y };
	//ClientToScreen(hwnd, &point);
	SetCursorPos(point.x,point.y);
	mousePos = setPos;
}
