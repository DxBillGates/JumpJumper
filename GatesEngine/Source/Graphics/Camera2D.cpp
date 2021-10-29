#include "..\..\Header\Graphics\Camera2D.h"
#include "..\..\Header\Graphics\CBufferAllocater.h"

GatesEngine::Camera2D::Camera2D()
{
}

GatesEngine::Camera2D::~Camera2D()
{
}

void GatesEngine::Camera2D::Set(int setDescriptorIndex)
{
	projectionMatrix = Math::Matrix4x4::GetOrthographMatrix(size);
	pGraphicsDevice->GetCBufferAllocater()->BindAndAttach(setDescriptorIndex,projectionMatrix);
}

void GatesEngine::Camera2D::SetSize(const Math::Vector2& s)
{
	size = s;
}
