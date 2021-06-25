#pragma once
#include "GraphicsDevice.h"
#include "Texture.h"
#include "DepthStencil.h"

namespace GatesEngine
{
	class DepthStencilTexture : public Texture,public DepthStencil
	{
	private:
		int srvNumber;
	public:
		DepthStencilTexture();
		~DepthStencilTexture();
		void Create(GraphicsDevice* pGraphicsDevice,const Math::Vector2& size);
		void Set(int descIndex) override;
	};
}
