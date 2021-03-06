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
	/// 画面揺れをセットする関数
	/// </summary>
	/// <param name="time">揺れさせる時間(秒)</param>
	/// <param name="length">揺れの大きさ(pixel)</param>
	void SetShake(float time, float length = 1);
};