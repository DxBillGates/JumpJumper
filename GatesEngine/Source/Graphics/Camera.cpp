#include "..\..\Header\Graphics\Camera.h"
#include "..\..\Header\Graphics\CBufferAllocater.h"
#include <DirectXMath.h>

GatesEngine::Camera::Camera()
	: pGraphicsDevice(nullptr)
	, pMainWindow(nullptr)
	, projectionMatrix(Math::Matrix4x4::Identity())
	, nearClip(1)
	, farClip(10000)
{
}

void GatesEngine::Camera::SetGraphicsDevice(GraphicsDevice* gDevice)
{
	pGraphicsDevice = gDevice;
}

void GatesEngine::Camera::SetMainWindow(Window* window)
{
	pMainWindow = window;
}

void GatesEngine::Camera::SetProjection(const Math::Matrix4x4& matrix)
{
	projectionMatrix = matrix;
}

void GatesEngine::Camera::SetClip(float n, float f)
{
	nearClip = n;
	farClip = f;
}