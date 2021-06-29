#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Input/Input.h"
#include "Header/Graphics/Camera.h"

class PlayerBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Math::Vector3 vel;
	bool isJump;
	int combo;
	GatesEngine::Input* input;
	GatesEngine::Camera* mainCamera;
	bool isAnimation;
	float animationTime;
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void SetCamera(GatesEngine::Camera* pCamera);
};
