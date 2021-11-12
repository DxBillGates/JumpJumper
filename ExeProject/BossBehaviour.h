#pragma once
#include "Header/Component/Behaviour.h"

class BossBehaviour : GatesEngine::Behaviour
{
private:
public:
	BossBehaviour();
	~BossBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
};

