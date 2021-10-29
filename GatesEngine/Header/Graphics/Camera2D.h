#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//2D用のカメラを想定したCamera2Dクラス
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