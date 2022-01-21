#pragma once
#include "ButtonUI.h"
#include "Header/Graphics/GraphicsDevice.h"
#include "Header/Input/Input.h"
#include <vector>

class TitleManager
{
private:
	GatesEngine::GraphicsDevice* graphicsDevice;
	GatesEngine::Input* input;
	float alpha;
	float buttonsAlpha;
	bool increaseAlphaFlag;
	bool decreaseAlphaFlag;
	bool increaseButtonsAlphaFlag;
	bool isTitlePause;

	GatesEngine::Math::Vector2 mouseButtonSize;

	std::vector<ButtonUI*> buttons;
	GatesEngine::Math::Vector2 initializeSize;
	ButtonUI* clickButton;
	bool selectInputDeviceIsMouse;
	bool selectPadButton;

	float selectUIAnimationValue;
private:
	ButtonUI* CheckHitButtons();
	ButtonUI* CheckHitPadButtonIsStart();
public:
	TitleManager();
	~TitleManager();
	void Initialize();
	void Update();
	void Draw();
	void LateDraw();
	bool GetIsTitlePause();
	void SetGraphicsDevice(GatesEngine::GraphicsDevice* gDevice);
	void AddButton(ButtonUI* button);
	ButtonUI* GetClickButton();
	void SetInitialiseButtonSize(const GatesEngine::Math::Vector2& size);
};

