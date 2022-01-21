#include "TitleManager.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

ButtonUI* TitleManager::CheckHitButtons()
{
	if (!input)return nullptr;

	GatesEngine::Math::Vector2 mousePos = input->GetMouse()->GetMousePos();
	ButtonUI* returnButton = nullptr;
	for (auto& b : buttons)
	{
		GatesEngine::Math::Vector2 buttonPos = b->GetPos();
		GatesEngine::Math::Vector2 buttonSize = b->GetScale();

		GatesEngine::Math::Vector2 checkPointTop = buttonPos - buttonSize / 2;
		GatesEngine::Math::Vector2 checkPointBottom = buttonPos + buttonSize / 2;

		GatesEngine::Math::Vector2 mouseTop = mousePos - mouseButtonSize / 2;
		GatesEngine::Math::Vector2 mouseBottom = mousePos + mouseButtonSize / 2;

		if (mouseBottom.x < checkPointTop.x)continue;
		if (mouseBottom.y < checkPointTop.y)continue;

		if (mouseTop.x > checkPointBottom.x)continue;
		if (mouseTop.y > checkPointBottom.y)continue;

		returnButton = b;
	}

	return returnButton;
}

TitleManager::TitleManager()
	: graphicsDevice(nullptr)
	, input(nullptr)
	, alpha(0)
	, buttonsAlpha(0)
	, increaseAlphaFlag(true)
	, decreaseAlphaFlag(false)
	, increaseButtonsAlphaFlag(false)
	, isTitlePause(true)
	, mouseButtonSize({})
	, initializeSize({})
	, clickButton(nullptr)
{
}

TitleManager::~TitleManager()
{
}

void TitleManager::Initialize()
{
	if (!input)input = GatesEngine::Input::GetInstance();
	alpha = 0;
	buttonsAlpha = 0;
	increaseAlphaFlag = true;
	decreaseAlphaFlag = false;
	increaseButtonsAlphaFlag = false;
	isTitlePause = true;
	clickButton = nullptr;

	mouseButtonSize = { 10,10 };

	for (auto& b : buttons)
	{
		//if (!b)continue;
		b->SetScale(initializeSize);
		b->SetAlpha(0);
		//break;
	}
}

void TitleManager::Update()
{
	if (!input)return;

	if (alpha > 1)
	{
		alpha = 1;
		increaseAlphaFlag = false;
	}
	if (alpha < 0)
	{
		alpha = 0;
		increaseButtonsAlphaFlag = true;
	}

	const float PER_FRAME = 1.0f / 60.0f;
	const float DECREASE_TIME = 1;

	if (decreaseAlphaFlag)alpha -= PER_FRAME / DECREASE_TIME;

	const float INCREASE_TIME = 1;
	if (increaseAlphaFlag)alpha += PER_FRAME / INCREASE_TIME;

	if (buttonsAlpha > 1)
	{
		increaseButtonsAlphaFlag = false;
		isTitlePause = false;
	}

	if (increaseButtonsAlphaFlag)
	{
		for (auto& b : buttons)
		{
			b->SetAlpha(buttonsAlpha);
		}
		buttonsAlpha += PER_FRAME;
	}

	if (!increaseAlphaFlag && input->GetKeyboard()->CheckHitKeyAll())
	{
		decreaseAlphaFlag = true;
	}

	if (!isTitlePause)
	{
		for (auto& b : buttons)b->SetScale(initializeSize);

		ButtonUI* hitButton = CheckHitButtons();
		if (!hitButton)return;

		hitButton->SetScale(initializeSize / 2);

		if (input->GetMouse()->GetCheckReleaseTrigger(GatesEngine::MouseButtons::LEFT_CLICK))clickButton = hitButton;
		//GatesEngine::Math::Vector2 preSize = hitButton->GetScale();
		//CheckHitUIToMouse();
	}
}

void TitleManager::Draw()
{
}

void TitleManager::LateDraw()
{
	if (!graphicsDevice)return;

	GatesEngine::TextureManager* textureManager = GatesEngine::ResourceManager::GetTextureManager();
	GatesEngine::ShaderManager* shaderManager = GatesEngine::ResourceManager::GetShaderManager();
	GatesEngine::MeshManager* meshManager = GatesEngine::ResourceManager::GetMeshManager();

	shaderManager->GetShader("DefaultSpriteShader")->Set();

	GatesEngine::Math::Vector2 scale = { 1000,1000 };
	GatesEngine::Math::Vector2 pos = { 1920 / 2,1080 / 2 };

	scale = mouseButtonSize;
	pos = input->GetMouse()->GetMousePos();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	meshManager->GetMesh("2DPlane")->Draw();

	scale = { 500,500 };
	pos = { 1920 / 2,1080 / 2 };
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ scale.x,scale.y,0 }) * GatesEngine::Math::Matrix4x4::Translate({ pos.x,pos.y,0 }));
	float a = GatesEngine::Math::Easing::EaseInSine(alpha);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, a));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	meshManager->GetMesh("2DPlane")->Draw();
}

bool TitleManager::GetIsTitlePause()
{
	return isTitlePause;
}

void TitleManager::SetGraphicsDevice(GatesEngine::GraphicsDevice* gDevice)
{
	graphicsDevice = gDevice;
}

void TitleManager::AddButton(ButtonUI* button)
{
	button->SetAlpha(0);
	buttons.push_back(button);
}

ButtonUI* TitleManager::GetClickButton()
{
	return clickButton;
}

void TitleManager::SetInitialiseButtonSize(const GatesEngine::Math::Vector2& size)
{
	initializeSize = size;
}
