#pragma once
#include "Header/Component/Behaviour.h"
class BlockBehaviour : public GatesEngine::Behaviour
{
private:
	float hp;
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
};

