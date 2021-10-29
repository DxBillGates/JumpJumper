#pragma once
#include "Camera.h"

namespace GatesEngine
{
	class Camera3D : public Camera
	{
	protected:
		Math::Matrix4x4 viewMatrix;

		Math::Vector3 position;
		Math::Vector3 direction;
		Math::Vector3 up;

		float yaw;
		float pitch;

		Math::Matrix4x4 rotation;
	public:
		Camera3D();
		virtual ~Camera3D();
		void Set(int setDescriptorIndex) override;
		void SetPosition(const Math::Vector3& pos);
		void SetDirection(const Math::Vector3& dir);
		void SetYaw(float value);
		void SetPitch(float value);
		const Math::Matrix4x4& GetRotation();
		const Math::Vector3& GetPosition();
	};
}
