#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Input/Input.h"
#include "Header/Graphics/Camera.h"
#include "PlayerBullet.h"

class PlayerBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Math::Vector3 vel;
	bool isJump;
	GatesEngine::Input* input;
	GatesEngine::Camera* mainCamera;
	float fuelValue;
	const float MAX_FUEL;
	const float CHARGE_FUEL;

	std::vector<PlayerBullet*> bullets;
public:
	PlayerBehaviour();
	~PlayerBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void OnCollision(GatesEngine::Collider* hitCollider) override;
	void SetCamera(GatesEngine::Camera* pCamera);
	GatesEngine::Camera* GetSetCamera();
	void AddBullet(PlayerBullet* newBullet);
};
