#pragma once
#include "Header/Component/Behaviour.h"
#include "PlayerBehaviour.h"

class PlayerSceneTransAnimation : public GatesEngine::Behaviour
{
private:
	PlayerBehaviour* playerBehaviour;
	GatesEngine::Camera* mainCamera;
	bool isAnimation;
	bool isCameraAnimation;
	float cameraAnimationTime;
	float waitTime;
	bool onceFlag;
	GatesEngine::Math::Vector3 oldCameraPos;
	GatesEngine::Math::Vector3 oldCameraDir;
public:
	void Start() override;
	void Update() override;
};
