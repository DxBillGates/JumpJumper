#include "PlayerCamera.h"

PlayerCamera::PlayerCamera()
	: input(nullptr)
	, moveSpeed(1)
	, isFocus(true)
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
			
			//const float range = 0.05f;
			//if (inputValue.x >= range)inputValue.x = range;
			//if (inputValue.x <= -range)inputValue.x = -range;
			//if (inputValue.y >= range)inputValue.y = range;
			//if (inputValue.y <= -range)inputValue.y = -range;

			//inputValue = inputValue.Normalize() * PER_FRAME;
			//float preyaw = yaw, preitch = pitch;
			//printf("%f,%f\n", yaw, pitch);

			pitch -= inputValue.y;
			yaw -= inputValue.x;

			//printf("%f,%f\n", yaw-preyaw, pitch - preitch);

			float minMaxPitch = Math::ConvertToRadian(89);
			if (pitch > minMaxPitch)pitch = minMaxPitch;
			if (pitch < -minMaxPitch)pitch = -minMaxPitch;

			Math::Vector3 moveVector;

			moveSpeed = input->GetKeyboard()->CheckHitKey(Keys::LSHIFT) ? 10.0f : 1.0f;

			moveVector = moveVector.Normalize();
			position += moveVector * moveSpeed;
		}
	}
}