#pragma once
#include "Header/Component/Behaviour.h"
class TrankingEnemyBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::GameObject* trakingGameObject;
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void SetTrakingGameObject(GatesEngine::GameObject* setObject);
};

