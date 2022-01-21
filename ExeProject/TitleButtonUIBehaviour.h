#pragma once
#include "ButtonUI.h"
#include "Header/Component/Behaviour.h"
#include "Header/Graphics/Texture.h"

class TitleButtonUIBehaviour : public ButtonUI,public GatesEngine::Behaviour
{
private:
	GatesEngine::Texture* texture;
public:
	TitleButtonUIBehaviour();
	~TitleButtonUIBehaviour();
	void Start() override;
	void Update() override;
	void OnLateDraw() override;
	void SetTexture(GatesEngine::Texture* setTexture);
};

