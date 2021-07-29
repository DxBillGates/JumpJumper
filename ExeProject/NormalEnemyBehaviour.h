#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Graphics/Camera.h"

class NormalEnemyBehaviour : public GatesEngine::Behaviour
{
private:
	float time;
	float delay;
	float hp;
	float r;
	float y;
	GatesEngine::Math::Vector3 moveVector;
	GatesEngine::Math::Vector3 targetPos;
public:
	NormalEnemyBehaviour();
	~NormalEnemyBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void Damage(float value = 1);
	float GetHP();
	void SetR(float value);
	void SetY(float value);
};