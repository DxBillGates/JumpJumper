#include "..\..\Header\Math\Transform.h"

GatesEngine::Math::Matrix4x4 GatesEngine::Transform::GetMatrix()
{
	return Math::Matrix4x4::Scale(scale) * Math::Matrix4x4::RotationZ(rotation.z) * Math::Matrix4x4::RotationX(rotation.x) * 
		   Math::Matrix4x4::RotationY(rotation.y) * Math::Matrix4x4::Translate(position);
}

GatesEngine::Math::Vector3 GatesEngine::Transform::GetEulerAngle()
{
	return Math::Vector3();
}

GatesEngine::Math::Vector3 GatesEngine::Transform::GetForward()
{
	GatesEngine::Math::Axis axis = GetMatrix().GetAxis();
	return axis.z;
}

GatesEngine::Math::Vector3 GatesEngine::Transform::GetRight()
{
	GatesEngine::Math::Axis axis = GetMatrix().GetAxis();
	return axis.x;
}

GatesEngine::Math::Vector3 GatesEngine::Transform::GetUp()
{
	GatesEngine::Math::Axis axis = GetMatrix().GetAxis();
	return axis.y;
}
