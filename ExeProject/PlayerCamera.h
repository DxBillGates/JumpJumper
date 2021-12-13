#pragma once
#include "Header/Graphics/Camera3D.h"
#include "Header/Input/Input.h"

class PlayerCamera : public GatesEngine::Camera3D
{
private:
	GatesEngine::Input* input;
	float moveSpeed;
	bool isFocus;

	bool isCameraShaking;
	float cameraShakingTime;
	float maxCameraShakingTime;
	float shakeValue;
private:
	void Shake();
public:
	PlayerCamera();
	~PlayerCamera();
	void Initialize() override;
	void Update() override;

	/// <summary>
	/// ��ʗh����Z�b�g����֐�
	/// </summary>
	/// <param name="time">�h�ꂳ���鎞��(�b)</param>
	/// <param name="length">�h��̑傫��(pixel)</param>
	void SetShake(float time, float length = 1);
};