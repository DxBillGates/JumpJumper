#pragma once
#include "ButtonUI.h"
#include "Header/Component/Behaviour.h"

class TitleButtonUIBehaviour : public ButtonUI,public GatesEngine::Behaviour
{
private:
public:
	TitleButtonUIBehaviour();
	~TitleButtonUIBehaviour();
	void Start() override;
	void Update() override;
	void OnLateDraw() override;
};

