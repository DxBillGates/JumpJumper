#pragma once
#include "Camera3D.h"

namespace GatesEngine
{
	class Input;
	//デバッグ中に使う自由移動可能なカメラを想定としたクラス、マウスを使ってカメラの視点移動や、
	//キーボードを使用して移動が可能
	class DebugCamera : public Camera3D
	{
	protected:
		Input* input;
		float moveSpeed;
	public:
		DebugCamera();
		virtual ~DebugCamera();
		void Initialize() override;
		void Update() override;
	};
}
