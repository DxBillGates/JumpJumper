#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//2D�p�̃J������z�肵��Camera2D�N���X
	class Camera2D : public Camera
	{
	protected:
		Math::Vector2 size;
	public:
		Camera2D();
		virtual ~Camera2D();
		void Set(int setDescriptorIndex) override;
		void SetSize(const Math::Vector2& s);
	};

}