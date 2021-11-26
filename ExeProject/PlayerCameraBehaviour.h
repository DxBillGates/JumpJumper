#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/Camera3D.h"

class PlayerCameraBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Camera3D* camera;
	GatesEngine::Math::Vector3 beforeFramePos;
public:
	PlayerCameraBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::Collider* hitCollider) override;
	//void SetCamera(GatesEngine::Camera3D* camera);
};

