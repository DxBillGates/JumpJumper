#include "TutorialUI.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/Manager/ResourceManager.h"

TutorialUI::TutorialUI()
	: gDevice(nullptr)
	, texture(nullptr)
	, pos({})
	, scale({})
	, scaleWeight({})
	, color({ 0,0,0,1 })
	, scaleAnimationFlag(false)
	, easeingTime(0)
{
}

void TutorialUI::Initialize()
{
	scaleAnimationFlag = false;
	easeingTime = 0;
}

void TutorialUI::Update()
{
	if (!scaleAnimationFlag)return;
	if (easeingTime >= 1)
	{
		easeingTime = 1;
		scaleAnimationFlag = false;
	}

	const float PER_FRAME = 1.0f / 60.0f;

	if (scaleAnimationFlag)
	{
		easeingTime += PER_FRAME;
	}
}

void TutorialUI::Draw()
{
	float easeingValue = GatesEngine::Math::Easing::EaseOutBack(easeingTime);
	GatesEngine::Math::Vector3 drawScale = GatesEngine::Math::Vector3::Lerp(scale, scaleWeight, easeingValue);

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	gDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale(drawScale) * GatesEngine::Math::Matrix4x4::Translate(pos));
	gDevice->GetCBufferAllocater()->BindAndAttach(1, color);
	gDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();
}

void TutorialUI::SetGraphicsDevice(GatesEngine::GraphicsDevice* device)
{
	gDevice = device;
}

void TutorialUI::SetTexture(GatesEngine::Texture* tex)
{
	texture = tex;
}

void TutorialUI::SetScaleWeight(const GatesEngine::Math::Vector3& vec)
{
	scaleWeight = vec;
}

void TutorialUI::SetColor(const GatesEngine::Math::Vector4& vec)
{
	color = vec;
}

void TutorialUI::SetPosition(const GatesEngine::Math::Vector3& p)
{
	pos = p;
}

void TutorialUI::SetScale(const GatesEngine::Math::Vector3& vec)
{
	scale = vec;
}

void TutorialUI::SetScaleAnimationFlag(bool flag)
{
	scaleAnimationFlag = flag;
}

bool TutorialUI::GetScaleAnimationFlag()
{
	return scaleAnimationFlag;
}
