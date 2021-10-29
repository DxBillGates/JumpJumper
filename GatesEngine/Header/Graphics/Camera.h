#pragma once
#include "..\..\Header\Math\Math.h"
#include "..\..\Header\Window\Window.h"
#include "..\..\Header\Input\Input.h"
#include "GraphicsDevice.h"
#include "CBuffer.h"
#include "CBufferStruct.h"
namespace GatesEngine
{
	//基本となるカメラクラス、基本定点カメラにしか使えない
	class Camera
	{
	protected:
		GraphicsDevice* pGraphicsDevice;
		Window* pMainWindow;

		Math::Matrix4x4 projectionMatrix;
		float nearClip;
		float farClip;
	public:
		Camera();
		virtual ~Camera() {}
		virtual void Initialize() {}
		virtual void Update() {}
		virtual void Set(int setDescriptorIndex) {}
		void SetGraphicsDevice(GraphicsDevice* gDevice);
		void SetMainWindow(Window* window);
		void SetProjection(const Math::Matrix4x4& matrix);
		void SetClip(float n = 1, float f = 10000);
	};
}