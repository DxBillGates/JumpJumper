#pragma once
#include "Header/Component/Behaviour.h"
#include "PlayerBehaviour.h"

class PlayerSceneTransAnimation : public GatesEngine::Behaviour
{
private:
	GatesEngine::Camera* mainCamera;
	bool isAnimation;
	bool isCameraAnimation;
	float cameraAnimationTime;
	float waitTime;
	bool onceFlag;
	GatesEngine::Math::Vector3 oldCameraPos;
	GatesEngine::Math::Vector3 oldCameraDir;
	GatesEngine::Math::Vector3 vel;
private:
	void Jump(GatesEngine::Math::Vector3 vec, bool flag);
public:
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void SetCamera(GatesEngine::Camera* sCamera);
};
