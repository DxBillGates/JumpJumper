#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Graphics/Camera.h"

class NormalEnemyBehaviour : public GatesEngine::Behaviour
{
private:
	float hp;
	float t;
public:
	NormalEnemyBehaviour();
	~NormalEnemyBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void Damage(float value = 1);
	float GetHP();
};