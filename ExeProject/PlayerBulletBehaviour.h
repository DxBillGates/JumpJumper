#pragma once
#include "Header/Component/Behaviour.h"
#include "PlayerBullet.h"

class PlayerBulletBehaviour : public GatesEngine::Behaviour,public PlayerBullet
{
private:
public:
	PlayerBulletBehaviour();
	~PlayerBulletBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
};