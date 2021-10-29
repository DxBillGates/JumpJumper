#pragma once
#include "Camera3D.h"

namespace GatesEngine
{
	class Input;
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
