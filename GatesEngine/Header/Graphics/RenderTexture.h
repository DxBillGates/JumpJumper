#pragma once
#include "GraphicsDevice.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "..\Math\Vector2.h"

namespace GatesEngine
{
	class RenderTexture : public Texture,public RenderTarget
	{
	public:
		RenderTexture();
		~RenderTexture();
		void Prepare() override;
		void Create(GraphicsDevice* graphicsDevice, const GatesEngine::Math::Vector2& size, const Math::Vector4& color = {0,0,0,1});
		void Set(int descIndex) override;
		void EndDraw();
	};
}