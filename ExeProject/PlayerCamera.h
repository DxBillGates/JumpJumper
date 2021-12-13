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
	/// ‰æ–Ê—h‚ê‚ğƒZƒbƒg‚·‚éŠÖ”
	/// </summary>
	/// <param name="time">—h‚ê‚³‚¹‚éŠÔ(•b)</param>
	/// <param name="length">—h‚ê‚Ì‘å‚«‚³(pixel)</param>
	void SetShake(float time, float length = 1);
};