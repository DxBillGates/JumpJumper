#pragma once
#include "GraphicsDevice.h"
#include <d3d12.h>

namespace GatesEngine
{
	class DepthStencil
	{
	protected:
		GraphicsDevice* graphicsDevice;
		ID3D12Resource* depthBuffer;
		ID3D12DescriptorHeap* heap;
	public:
		DepthStencil();
		virtual ~DepthStencil();
		void CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);
		ID3D12Resource* GetResource();
		ID3D12DescriptorHeap* GetHeap();
		void Clear();
	};
}
