#include "PlayerCameraBehaviour.h"
#include "Header/GameObject/GameObject.h"

PlayerCameraBehaviour::PlayerCameraBehaviour()
	: camera(nullptr)
	, beforeFramePos({})
{
}

void PlayerCameraBehaviour::Start()
{
	camera = dynamic_cast<GatesEngine::Camera3D*>(gameObject->GetGraphicsDevice()->GetMainCamera());
}

void PlayerCameraBehaviour::Update()
{
	if (!camera)return;
	beforeFramePos = gameObject->GetTransform()->position;

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
		GatesEngine::Math::Vector3 fixPos = beforeFramePos;
		gameObject->GetTransform()->position = beforeFramePos;
		camera->SetPosition(beforeFramePos);
	}
}

//void PlayerCameraBehaviour::SetCamera(GatesEngine::Camera3D* camera)
//{
//	this->camera = camera;
//}
