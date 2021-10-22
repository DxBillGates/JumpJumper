#include "..\..\Header\Graphics\Camera.h"
#include <DirectXMath.h>

GatesEngine::Camera::Camera()
	: pGraphicsDevice(nullptr)
	, pMainWindow(nullptr)
	, input(Input::GetInstance())
	, cameraBuffer(CBuffer<B2>())
	, viewMatrix(Math::Matrix4x4::Identity())
	, projectionMatrix(Math::Matrix4x4::Identity())
	, position(Math::Vector3(0, 0, 0))
	, direction(Math::Vector3(0, 0, 1))
	, up(Math::Vector3(0, 1, 0))
	, isCreateBuffer(false)
	, yaw(0)
	, pitch(0)
	, moveSpeed(1)
	, rotation(Math::Matrix4x4::Identity())
	, isFocus(true)
{
}

GatesEngine::Camera::~Camera()
{
}

void GatesEngine::Camera::Initialize()
{
	viewMatrix = Math::Matrix4x4::Identity();
	projectionMatrix = Math::Matrix4x4::GetPerspectiveMatrix(90, pMainWindow->GetWindowAspect(),1,10000);

	position = Math::Vector3(0, 10, -200);
	direction = Math::Vector3(0, 0, 1);
	up = Math::Vector3(0, 1, 0);
	rotation = Math::Matrix4x4::Identity();
}

void GatesEngine::Camera::Update()
{
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
		//マウスの移動量を取得
		//if (input->GetMouse()->GetCheckHitButton(MouseButtons::RIGHT_CLICK))
		//{
		input->GetMouse()->SetMouseCursor({ 1920 / 2,1080 / 2 });
		//Math::Vector2 inputPos = input->GetMouse()->GetMousePos();
		//Math::Vector2 newPos;
		//newPos.x = inputPos.x <= 0 ? 1920 : inputPos.x;
		//newPos.x = inputPos.x > 1920 ? 0: newPos.x;
		//newPos.y = inputPos.y <= 0 ? 1070 : inputPos.y;
		//newPos.y = inputPos.y > 1070 ? 0 : newPos.y;
		//if (inputPos.x != newPos.x || inputPos.y != newPos.y)
		//{
		//	input->GetMouse()->SetMouseCursor({ newPos.x,newPos.y});
		//}

		Math::Vector2 inputValue = input->GetMouse()->GetMouseMove() / 500.0f;

		pitch -= inputValue.y;
		yaw -= inputValue.x;

		float minMaxPitch = Math::ConvertToRadian(89);
		if (pitch > minMaxPitch)pitch = minMaxPitch;
		if (pitch < -minMaxPitch)pitch = -minMaxPitch;

		Math::Vector3 moveVector;

		//if (input->GetKeyboard()->CheckHitKey(Keys::W))
		//{
		//	moveVector += axis.z;
		//}
		//if (input->GetKeyboard()->CheckHitKey(Keys::S))
		//{
		//	moveVector -= axis.z;
		//}
		//if (input->GetKeyboard()->CheckHitKey(Keys::D))
		//{
		//	moveVector += axis.x;
		//}
		//if (input->GetKeyboard()->CheckHitKey(Keys::A))
		//{
		//	moveVector -= axis.x;
		//}
		//if (input->GetKeyboard()->CheckHitKey(Keys::E))
		//{
		//	moveVector += axis.y;
		//}
		//if (input->GetKeyboard()->CheckHitKey(Keys::Q))
		//{
		//	moveVector -= axis.y;
		//}

		moveSpeed = input->GetKeyboard()->CheckHitKey(Keys::LSHIFT) ? 10.0f : 1.0f;

		moveVector = moveVector.Normalize();
		position += moveVector * moveSpeed;
		//}

		direction = Math::Vector3(0, 0, 1);
		rotation = Math::Matrix4x4::RotationX(pitch) * Math::Matrix4x4::RotationY(yaw);
		direction = Math::Matrix4x4::Transform(direction, rotation);

		viewMatrix = Math::Matrix4x4::GetViewMatrixLookTo(position, direction, axis.y);

		cameraBuffer.Map({ viewMatrix,projectionMatrix });
	}
}

void GatesEngine::Camera::Set()
{
	cameraBuffer.Set();
}

void GatesEngine::Camera::SetGraphicsDevice(GraphicsDevice* device)
{
	if (!isCreateBuffer)
	{
		cameraBuffer.Create(device, 2);
	}
	pGraphicsDevice = device;
	isCreateBuffer = true;
}

void GatesEngine::Camera::SetMainWindow(Window* mainWindow)
{
	pMainWindow = mainWindow;
}

void GatesEngine::Camera::SetPosition(const Math::Vector3& pos)
{
	position = pos;
}

void GatesEngine::Camera::SetDirection(const Math::Vector3& dir)
{
	direction = dir;
}

void GatesEngine::Camera::SetYawPitch(const Math::Vector2& value)
{
	yaw = value.x;
	pitch = value.y;
}

GatesEngine::B2 GatesEngine::Camera::GetData()
{
	B2 data = { viewMatrix,projectionMatrix,position };
	return data;
}

GatesEngine::Math::Matrix4x4* GatesEngine::Camera::GetRotation()
{
	return &rotation;
}

GatesEngine::Math::Vector3 GatesEngine::Camera::GetPosition()
{
	return position;
}

GatesEngine::Math::Vector3 GatesEngine::Camera::GetYawPitchRow()
{
	return {yaw,pitch,0};
}
