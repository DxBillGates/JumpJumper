#include "PlayerSceneTransAnimation.h"
#include "Header/GameObject/GameObject.h"

void PlayerSceneTransAnimation::Start()
{
	playerBehaviour = nullptr;
	mainCamera = nullptr;
	isAnimation = false;
	isCameraAnimation = false;
	cameraAnimationTime = 0;
	waitTime = 0;
	onceFlag = false;
}

void PlayerSceneTransAnimation::Update()
{
	if (!mainCamera)
	{
		playerBehaviour = gameObject->GetComponent<PlayerBehaviour>();
		if (playerBehaviour)
		{
			mainCamera = playerBehaviour->GetSetCamera();
		}
	}

	if (!onceFlag && playerBehaviour->GetSceneTransFlag() && GatesEngine::Math::Vector3::Distance({},gameObject->GetTransform()->position) <= 100)
	{
		onceFlag = true;
		isCameraAnimation = true;
		playerBehaviour->SetEnabled(false);
		oldCameraPos = mainCamera->GetPosition();
		oldCameraDir = mainCamera->GetYawPitchRow();
	}

	if (isCameraAnimation)
	{
		cameraAnimationTime += 0.016f;

		if (cameraAnimationTime >= 1)cameraAnimationTime = 1;

		using namespace GatesEngine;
		Math::Vector3 pos = gameObject->GetTransform()->position + Math::Vector3(0,100,-300);
		mainCamera->SetPosition(Math::Vector3::Lerp(oldCameraPos, pos,(cameraAnimationTime)));
		mainCamera->SetYawPitch(Math::Vector2::Lerp({ oldCameraDir.x,oldCameraDir.y }, { Math::ConvertToRadian(-10),Math::ConvertToRadian(10)}, Math::Easing::EaseInCirc(cameraAnimationTime)));

		if (cameraAnimationTime >= 1)
		{
			isCameraAnimation = false;
		}
	}

	if (!isCameraAnimation && cameraAnimationTime >= 1 && !isAnimation)
	{
		waitTime += 0.016f;
		if (waitTime >= 1)
		{
			isAnimation = true;
			using namespace GatesEngine;
			Math::Vector3 vec = Math::Vector3() - gameObject->GetTransform()->position;
			vec = vec.Normalize() * 3;
			vec.y = 10;
			playerBehaviour->Jump(vec, true);
		}
	}

	if (isAnimation)
	{
		playerBehaviour->Jump({}, false);
	}
}
