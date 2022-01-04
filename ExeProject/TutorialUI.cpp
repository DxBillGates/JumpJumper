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
	endStartFlag = true;
	startEndFlag = false;
	startWeightAnimationFlag = true;
	startHeightAnimationFlag = false;
	startAndEndWeightAnimationTime = 0;
	startAndEndHeightAnimationTime = 0;
	endHeightAnimationFlag = false;
	endWeightAnimationFlag = false;
	easeingTime = 0;
	firstEndFlag = false;
}

void TutorialUI::Update()
{
	//if (!scaleAnimationFlag)return;
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

	const float MAX_WEIGHT_ANIMATION_TIME = 1;
	const float MAX_HEIGHT_ANIMATION_TIME = 1;
	if (startEndFlag)
	{
		if (endHeightAnimationFlag && startAndEndHeightAnimationTime <= 0)
		{
			startAndEndHeightAnimationTime = 0;
			endHeightAnimationFlag = false;
			endWeightAnimationFlag = true;
		}
		else if (endWeightAnimationFlag && startAndEndWeightAnimationTime <= 0)
		{
			startAndEndWeightAnimationTime = 0;
			endWeightAnimationFlag = false;
			startEndFlag = false;
		}

		if (endHeightAnimationFlag)
		{
			startAndEndHeightAnimationTime -= PER_FRAME / MAX_HEIGHT_ANIMATION_TIME;
		}
		else if (endWeightAnimationFlag)
		{
			startAndEndWeightAnimationTime -= PER_FRAME / MAX_WEIGHT_ANIMATION_TIME;
		}
	}

	if (endStartFlag)
	{
		if (startAndEndWeightAnimationTime >= 1)
		{
			startWeightAnimationFlag = false;
			startAndEndWeightAnimationTime = 1;
			startHeightAnimationFlag = true;
		}
		if (startAndEndHeightAnimationTime >= 1)
		{
			startHeightAnimationFlag = false;
			startAndEndHeightAnimationTime = 1;
			endStartFlag = false;
		}
	}
	if (startWeightAnimationFlag)startAndEndWeightAnimationTime += PER_FRAME / MAX_WEIGHT_ANIMATION_TIME;
	else if (startHeightAnimationFlag)startAndEndHeightAnimationTime += PER_FRAME / MAX_HEIGHT_ANIMATION_TIME;
}

void TutorialUI::Draw()
{
	float easeingValue = GatesEngine::Math::Easing::EaseOutBack(easeingTime);

	GatesEngine::Math::Vector3 drawScale;
	if (startWeightAnimationFlag)
	{
		easeingValue = GatesEngine::Math::Easing::EaseOutBack(startAndEndWeightAnimationTime);
		drawScale = GatesEngine::Math::Vector3::Lerp({ 0 }, { scale.x,scale.y / 10,0 }, easeingValue);
	}
	else if (startHeightAnimationFlag)
	{
		easeingValue = GatesEngine::Math::Easing::EaseOutBack(startAndEndHeightAnimationTime);
		drawScale = GatesEngine::Math::Vector3::Lerp({ scale.x,0,0 }, { scale.x,scale.y,0 }, easeingValue);
	}
	else if (endHeightAnimationFlag)
	{
		easeingValue = GatesEngine::Math::Easing::EaseOutBack(1-startAndEndHeightAnimationTime);
		drawScale = GatesEngine::Math::Vector3::Lerp({ scale.x,scale.y,0 } ,{ scale.x,scale.y / 10,0 }, easeingValue);
	}
	else if (endWeightAnimationFlag)
	{
		easeingValue = GatesEngine::Math::Easing::EaseOutBack(1-startAndEndWeightAnimationTime);
		drawScale = GatesEngine::Math::Vector3::Lerp({ scale.x,scale.y / 10,0 } ,{ 0 }, easeingValue);
	}
	else
	{
		easeingValue = GatesEngine::Math::Easing::EaseOutBack(easeingTime);
		drawScale = GatesEngine::Math::Vector3::Lerp(scale, scaleWeight, easeingValue);
	}

	if (!endHeightAnimationFlag && !endWeightAnimationFlag && firstEndFlag)
	{
		drawScale = {};
	}

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

void TutorialUI::SetEndFlag(bool flag)
{
	if (!firstEndFlag)
	{
		startEndFlag = flag;
		endHeightAnimationFlag = flag;
		firstEndFlag = flag;
	}
}

bool TutorialUI::GetScaleAnimationFlag()
{
	return scaleAnimationFlag;
}

bool TutorialUI::GetStartFlag()
{
	return endStartFlag;
}
