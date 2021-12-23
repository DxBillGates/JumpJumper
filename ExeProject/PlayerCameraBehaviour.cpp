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
	cameraDistance = DEFAULT_CAMERA_DISTANCE;
}

void PlayerCameraBehaviour::Update()
{
	if (!camera)return;
	if (!playerObject)return;

	//if (cameraDistance >= DEFAULT_CAMERA_DISTANCE)cameraDistance = DEFAULT_CAMERA_DISTANCE;
	//beforeFramePos = gameObject->GetTransform()->position;
	//GatesEngine::Utility::Printf("cDistance : %f\n", cameraDistance);
	// カメラのポジション更新
	GatesEngine::Math::Vector3 pos = playerObject->GetTransform()->position;
	GatesEngine::Math::Axis cAxis = camera->GetRotation().GetAxis();
	camera->SetPosition({ GatesEngine::Math::Vector3(pos.x,pos.y + 100,pos.z) - cAxis.z * cameraDistance });
	gameObject->GetTransform()->position = camera->GetPosition();
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
		//cameraDistance = 0;
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
