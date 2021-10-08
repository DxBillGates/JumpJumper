#include "SceneTranslater.h"

SceneTranslater::SceneTranslater()
	: setTime(0)
	, time(0)
	, scale({})
	, state(TranslateState::NONE)
	, isActive(false)
{
}

void SceneTranslater::StartSceneTranslate(int time)
{
	setTime = time;
	isActive = true;
	switch (state)
	{
	case SceneTranslater::TranslateState::NONE:
		return;
	case SceneTranslater::TranslateState::UP:
		this->time = 0;
		break;
	case SceneTranslater::TranslateState::DOWN:
		this->time = (float)setTime;
		break;
	}
}

void SceneTranslater::Update(float elapsedTime)
{
	switch (state)
	{
	case TranslateState::NONE:
		return;
	case TranslateState::UP:
		time += elapsedTime;
		break;
	case TranslateState::DOWN:
		time -= elapsedTime;
		break;
	}

	if (time > setTime || time < 0)
	{
		time = (state == TranslateState::UP) ? (float)setTime : 0;
		state = (state == TranslateState::UP) ? TranslateState::DOWN : TranslateState::NONE;
		isActive = false;
	}

	scale = GatesEngine::Math::Vector3::Lerp({}, { 16,16,16 },GatesEngine::Math::Easing::EaseInCubic(time));
}

void SceneTranslater::Draw(GatesEngine::GraphicsDevice* graphicsDevice)
{
	if (isActive)
	{
		graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		graphicsDevice->GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale(scale) * GatesEngine::Math::Matrix4x4::Translate({1920/2,1080/2,0}));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(0, 0, 0, 1));
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
		graphicsDevice->GetMeshManager()->GetMesh("2DPlane")->Draw();
	}
}

bool SceneTranslater::IsActive()
{
	return isActive;
}

void SceneTranslater::SetIsActive(bool setFlag)
{
	isActive = setFlag;
}

void SceneTranslater::SetTranslateState(TranslateState setState)
{
	state = setState;
}

SceneTranslater::TranslateState SceneTranslater::GetTranslateState()
{
	return state;
}
