#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Input/Input.h"
#include "Header/Graphics/Camera.h"

class PlayerBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Math::Vector3 pos;
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Input* input;
	GatesEngine::Camera* mainCamera;
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void SetCamera(GatesEngine::Camera* pCamera);
};