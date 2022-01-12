#include "PlayerCameraBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Util/Utility.h"

PlayerCameraBehaviour::PlayerCameraBehaviour()
	: camera(nullptr)
	, beforeFramePos({})
	, playerObject(nullptr)
	, DEFAULT_CAMERA_DISTANCE(300)
	, cameraDistance(DEFAULT_CAMERA_DISTANCE)
{
}

void PlayerCameraBehaviour::Start()
{
	camera = dynamic_cast<GatesEngine::Camera3D*>(gameObject->GetGraphicsDevice()->GetMainCamera());
	camera->SetDirection({ 0,1,0 });
	cameraDistance = DEFAULT_CAMERA_DISTANCE;
	oldCameraDistance = cameraDistance;
}

void PlayerCameraBehaviour::Update()
{
	if (!camera)return;
	if (!playerObject)return;

	oldCameraDistance = cameraDistance;
	beforeFramePos = gameObject->GetTransform()->position;

	// カメラのポジション更新
	GatesEngine::Math::Vector3 pos = playerObject->GetTransform()->position;
	GatesEngine::Math::Axis cAxis = camera->GetRotation().GetAxis();

	// プレイヤーとカメラの距離
	float cameraToPlayerDis = GatesEngine::Math::Vector3::Distance(beforeFramePos, pos);
	float cameraDistanceDiff = cameraToPlayerDis - cameraDistance;
	//GatesEngine::Utility::Printf("%f\n", cameraDistanceDiff);

	GatesEngine::Math::Vector3 newCameraPos = GatesEngine::Math::Vector3(pos.x, pos.y + 100, pos.z) - cAxis.z * cameraDistance;

	const float NORMAL_LERP_VALUE = 0.1f;

	GatesEngine::Math::Vector3 lerpCameraPos = GatesEngine::Math::Vector3::Lerp(beforeFramePos, newCameraPos, NORMAL_LERP_VALUE);

	GatesEngine::Math::Vector3 addCameraPos = -cAxis.z * cameraDistanceDiff;
	if (cameraDistanceDiff <= 0)addCameraPos = {};

	camera->SetPosition(lerpCameraPos + addCameraPos);
	gameObject->GetTransform()->position = lerpCameraPos;
}

void PlayerCameraBehaviour::OnDraw()
{
}

void PlayerCameraBehaviour::OnLateDraw()
{
}

void PlayerCameraBehaviour::OnCollision(GatesEngine::Collider* hitCollider)
{
	if (hitCollider->GetGameObject()->GetTag() == "block")
	{
		//GatesEngine::Math::Vector3 fixPos = beforeFramePos;
		//gameObject->GetTransform()->position = beforeFramePos;
		//camera->SetPosition(beforeFramePos);
		//cameraDistance-=1;
	}
}

void PlayerCameraBehaviour::SetPlayerObject(GatesEngine::GameObject* object)
{
	playerObject = object;
}

//void PlayerCameraBehaviour::SetCamera(GatesEngine::Camera3D* camera)
//{
//	this->camera = camera;
//}
