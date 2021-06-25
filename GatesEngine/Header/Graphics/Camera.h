#pragma once
#include "..\..\Header\Math\Math.h"
#include "..\..\Header\Window\Window.h"
#include "..\..\Header\Input\Input.h"
#include "GraphicsDevice.h"
#include "CBuffer.h"
#include "CBufferStruct.h"
namespace GatesEngine
{
	class Camera
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		Window* pMainWindow;
		Input* input;
		CBuffer<B2> cameraBuffer;

		Math::Matrix4x4 viewMatrix;
		Math::Matrix4x4 projectionMatrix;

		Math::Vector3 position;
		Math::Vector3 direction;
		Math::Vector3 up;
		bool isCreateBuffer;

		float yaw;
		float pitch;
		float moveSpeed;

		Math::Matrix4x4 rotation;
	public:
		Camera();
		~Camera();
		void Initialize();
		void Update();
		void Set();
		void SetGraphicsDevice(GraphicsDevice* device);
		void SetMainWindow(Window* mainWindow);
		B2 GetData();
		Math::Matrix4x4* GetRotation();
	};
}