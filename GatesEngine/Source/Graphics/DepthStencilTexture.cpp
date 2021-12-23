#include "..\..\Header\Graphics\DepthStencilTexture.h"
#include "..\..\Header\Graphics\Graphics.h"

GatesEngine::DepthStencilTexture::DepthStencilTexture()
	: DepthStencil()
	, Texture()
{
}

GatesEngine::DepthStencilTexture::~DepthStencilTexture()
{
}

void GatesEngine::DepthStencilTexture::Create(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size)
{
	CreateBuffer(pGraphicsDevice, size);
	srvNumber = graphicsDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	graphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(depthBuffer);
}

void GatesEngine::DepthStencilTexture::Set(int descIndex)
{
	graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, graphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(srvNumber));
}
