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
			Math::Vector2 inputValue = input->GetMouse()->GetMouseMove() / 500.0f;

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
}