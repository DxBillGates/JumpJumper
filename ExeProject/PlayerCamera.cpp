#include "PlayerCamera.h"
#include "Header/Util/Random.h"

void PlayerCamera::Shake()
{
	if (!isCameraShaking)return;
	if (cameraShakingTime > 1)
	{
		isCameraShaking = false;
		cameraShakingTime = 0;
		maxCameraShakingTime = 0;
		addVector = {};
		return;
	}

	GatesEngine::Math::Axis axis = rotation.GetAxis();
	float range = 32767;
	GatesEngine::Math::Vector3 randomVector;
	randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) ,0 };

	randomVector = randomVector.Normalize();

	GatesEngine::Math::Vector3 xVector = axis.x * randomVector.x;
	GatesEngine::Math::Vector3 yVector = axis.y * randomVector.y;
	GatesEngine::Math::Vector3 vector = xVector + yVector;
	addVector = vector.Normalize() * shakeValue;

	const float PER_FRAME = 1.0f / 60.0f;
	cameraShakingTime += PER_FRAME / maxCameraShakingTime;
}

PlayerCamera::PlayerCamera()
	: input(nullptr)
	, moveSpeed(1)
	, isFocus(true)
	, isCameraShaking(false)
	, cameraShakingTime(0)
	, maxCameraShakingTime(0)
	, shakeValue(0)
{
	Initialize();
}

PlayerCamera::~PlayerCamera()
{
}

void PlayerCamera::Initialize()
{
	input = GatesEngine::Input::GetInstance();
}

void PlayerCamera::Update()
{
	using namespace GatesEngine;
	Math::Axis axis = rotation.GetAxis();

	if (input->GetKeyboard()->CheckPressTrigger(Keys::F1))
	{
		isFocus = false;
	}
	if (input->GetKeyboard()->CheckPressTrigger(Keys::F2))
	{
		isFocus = true;
	}
	if (isFocus)
	{
		if (input->GetIsFocus())
		{
			input->GetMouse()->SetMouseCursor({ 1920 / 2,1080 / 2 });
			//ƒ}ƒEƒX‚ÌˆÚ“®—Ê‚ðŽæ“¾
			const float PER_FRAME = 1.0f / 60.0f;
			Math::Vector2 inputValue = input->GetMouse()->GetMouseMove() * PER_FRAME / 10;

			pitch -= inputValue.y;
			yaw -= inputValue.x;

			float minMaxPitch = Math::ConvertToRadian(89);
			if (pitch > minMaxPitch)pitch = minMaxPitch;
			if (pitch < -minMaxPitch)pitch = -minMaxPitch;

			Math::Vector3 moveVector;

			moveSpeed = input->GetKeyboard()->CheckHitKey(Keys::LSHIFT) ? 10.0f : 1.0f;

			moveVector = moveVector.Normalize();
			position += moveVector * moveSpeed;
		}
	}

	if (isCameraShaking)Shake();
}

void PlayerCamera::SetShake(float time, float length)
{
	isCameraShaking = true;
	cameraShakingTime = 0;
	maxCameraShakingTime = time;
	shakeValue = length;
}
