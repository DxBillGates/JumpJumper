#include "..\..\Header\Graphics\DebugCamera.h"
#include "..\..\Header\Input\Input.h"

GatesEngine::DebugCamera::DebugCamera()
	: input(nullptr)
	, moveSpeed(1)
{
	Initialize();
}

GatesEngine::DebugCamera::~DebugCamera()
{
}

void GatesEngine::DebugCamera::Initialize()
{
	input = Input::GetInstance();
}

void GatesEngine::DebugCamera::Update()
{
	Math::Axis axis = rotation.GetAxis();

	float wheelValue = input->GetMouse()->GetWheelValue();
	Math::Vector3 moveVector;
	if (input->GetMouse()->GetCheckHitButton(MouseButtons::RIGHT_CLICK))
	{
		if (input->GetIsFocus())
		{
			input->GetMouse()->SetMouseCursor({ 1920 / 2,1080 / 2 });
			//マウスの移動量を取得
			Math::Vector2 inputValue = input->GetMouse()->GetMouseMove() / 500.0f;

			pitch -= inputValue.y;
			yaw -= inputValue.x;

			float minMaxPitch = Math::ConvertToRadian(89);
			if (pitch > minMaxPitch)pitch = minMaxPitch;
			if (pitch < -minMaxPitch)pitch = -minMaxPitch;


			if (input->GetKeyboard()->CheckHitKey(Keys::W))
			{
				moveVector += axis.z;
			}
			if (input->GetKeyboard()->CheckHitKey(Keys::S))
			{
				moveVector -= axis.z;
			}
			if (input->GetKeyboard()->CheckHitKey(Keys::D))
			{
				moveVector += axis.x;
			}
			if (input->GetKeyboard()->CheckHitKey(Keys::A))
			{
				moveVector -= axis.x;
			}
			if (input->GetKeyboard()->CheckHitKey(Keys::E))
			{
				moveVector += axis.y;
			}
			if (input->GetKeyboard()->CheckHitKey(Keys::Q))
			{
				moveVector -= axis.y;
			}
		}
	}
	moveSpeed = input->GetKeyboard()->CheckHitKey(Keys::LSHIFT) ? 10.0f : 1.0f;

	moveVector = moveVector.Normalize();
	moveVector += wheelValue * axis.z.Normalize();
	position += moveVector * moveSpeed;
}
