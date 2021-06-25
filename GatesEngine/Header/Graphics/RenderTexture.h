#pragma once
#include "GraphicsDevice.h"
#include "Texture.h"
#include "RenderTarget.h"
#include "..\Math\Vector2.h"

namespace GatesEngine
{
	class RenderTexture : public Texture,public RenderTarget
	{
	private:
		//Texture�N���X�����Ă��Ȃ����ߎ����I��
		ID3D12Resource* texBuff;
		int srvNumber;
	public:
		RenderTexture();
		~RenderTexture();
		void Prepare() override;
		void Create(GraphicsDevice* graphicsDevice,const GatesEngine::Math::Vector2& size);
		void Set(int descIndex) override;
		void EndDraw();
	};
}