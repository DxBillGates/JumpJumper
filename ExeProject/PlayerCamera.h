#pragma once
#include "Header/Graphics/Camera3D.h"
#include "Header/Input/Input.h"

class PlayerCamera : public GatesEngine::Camera3D
{
private:
	GatesEngine::Input* input;
	float moveSpeed;
	bool isFocus;
public:
	PlayerCamera();
	~PlayerCamera();
	void Initialize() override;
	void Update() override;
};