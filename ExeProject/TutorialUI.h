#pragma once
#include "Header/Math/Vector3.h"
#include "Header/Math/Vector4.h"
#include "Header/Graphics/Texture.h"
class TutorialUI
{
private:
	GatesEngine::GraphicsDevice* gDevice;
	GatesEngine::Texture* texture;
	GatesEngine::Math::Vector3 pos;
	GatesEngine::Math::Vector3 scale;
	GatesEngine::Math::Vector3 scaleWeight;
	GatesEngine::Math::Vector4 color;
	bool scaleAnimationFlag;
	float easeingTime;
public:
	TutorialUI();
	void Initialize();
	void Update();
	void Draw();
	void SetGraphicsDevice(GatesEngine::GraphicsDevice* device);
	void SetTexture(GatesEngine::Texture* tex);
	void SetPosition(const GatesEngine::Math::Vector3& p);
	void SetScale(const GatesEngine::Math::Vector3& vec);
	void SetScaleWeight(const GatesEngine::Math::Vector3& vec);
	void SetColor(const GatesEngine::Math::Vector4& vec);
	void SetScaleAnimationFlag(bool flag);
	bool GetScaleAnimationFlag();
};

