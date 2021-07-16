#include "PlayerSceneTransAnimation.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"

void PlayerSceneTransAnimation::Jump(GatesEngine::Math::Vector3 vec, bool flag)
{
	if (flag)vel = vec;
	vel -= GatesEngine::Math::Vector3(0, 0.981f / 2, 0);

	gameObject->GetTransform()->position += vel;
}

void PlayerSceneTransAnimation::Start()
{
	isAnimation = false;
	isCameraAnimation = false;
	cameraAnimationTime = 0;
	waitTime = 0;
	onceFlag = false;
	gameObject->GetTransform()->position = {10,0,-10};
}

void PlayerSceneTransAnimation::Update()
{
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::SPACE))
	{
		onceFlag = true;
		isCameraAnimation = true;
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

		float yaw = 0;
		yaw = gameObject->GetTransform()->position.x < 0 ? 10 : -10;
		mainCamera->SetYawPitch(Math::Vector2::Lerp({ oldCameraDir.x,oldCameraDir.y }, { Math::ConvertToRadian(yaw),Math::ConvertToRadian(10)}, Math::Easing::EaseInCirc(cameraAnimationTime)));

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
			Jump(vec, true);
		}
	}

	if (isAnimation)
	{
		Jump({}, false);
	}
}

void PlayerSceneTransAnimation::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	graphicsDevice->GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, mainCamera->GetData());
	graphicsDevice->GetMeshManager()->GetMesh("Cube")->Draw();
}

void PlayerSceneTransAnimation::SetCamera(GatesEngine::Camera* sCamera)
{
	mainCamera = sCamera;
}
