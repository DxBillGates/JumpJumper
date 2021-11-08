#include "..\..\Header\Graphics\Camera3D.h"
#include "..\..\Header\Graphics\CBufferAllocater.h"

GatesEngine::Camera3D::Camera3D()
	: viewMatrix(Math::Matrix4x4::Identity())
	, position(0)
	, direction({0,0,1})
	, up({0,1,0})
	, yaw(0)
	, pitch(0)
{
}

GatesEngine::Camera3D::~Camera3D()
{
}

void GatesEngine::Camera3D::Set(int setDescriptorIndex)
{
	projectionMatrix = Math::Matrix4x4::GetPerspectiveMatrix(90, pMainWindow->GetWindowAspect(),1,20000);

	direction = Math::Vector3(0, 0, 1);
	rotation = Math::Matrix4x4::RotationX(pitch) * Math::Matrix4x4::RotationY(yaw);
	direction = Math::Matrix4x4::Transform(direction, rotation);
	Math::Axis axis = rotation.GetAxis();
	viewMatrix = Math::Matrix4x4::GetViewMatrixLookTo(position, direction, axis.y);
	pGraphicsDevice->GetCBufferAllocater()->BindAndAttach(setDescriptorIndex, B2{ viewMatrix,projectionMatrix,position,Math::Matrix4x4::RotationX(pitch) * Math::Matrix4x4::RotationY(yaw) });
}

void GatesEngine::Camera3D::SetPosition(const Math::Vector3& pos)
{
	position = pos;
}

void GatesEngine::Camera3D::SetDirection(const Math::Vector3& dir)
{
	yaw = atan2f(dir.x, dir.z);
	pitch = atan2f(dir.z, dir.y);

	float minMaxPitch = Math::ConvertToRadian(89);
	if (pitch > minMaxPitch)pitch = minMaxPitch;
	if (pitch < -minMaxPitch)pitch = -minMaxPitch;
}

void GatesEngine::Camera3D::SetYaw(float value)
{
	yaw = value;
}

void GatesEngine::Camera3D::SetPitch(float value)
{
	pitch = value;
	float minMaxPitch = Math::ConvertToRadian(89);
	if (pitch > minMaxPitch)pitch = minMaxPitch;
	if (pitch < -minMaxPitch)pitch = -minMaxPitch;
}

const GatesEngine::Math::Matrix4x4& GatesEngine::Camera3D::GetRotation()
{
	return rotation;
}

const GatesEngine::Math::Vector3& GatesEngine::Camera3D::GetPosition()
{
	return position;
}
