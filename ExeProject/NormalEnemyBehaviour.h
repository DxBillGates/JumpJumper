#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"

class NormalEnemyBehaviour : public GatesEngine::Behaviour
{
private:
	float time;
	float delay;
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
};