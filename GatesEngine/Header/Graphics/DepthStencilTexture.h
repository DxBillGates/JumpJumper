#pragma once
#include "GraphicsDevice.h"
#include "Texture.h"
#include "DepthStencil.h"

namespace GatesEngine
{
	class DepthStencilTexture : public Texture,public DepthStencil
	{
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DepthStencilTexture();
		~DepthStencilTexture();

		/// <summary>
		/// 深度テクスチャ生成関数
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
		/// <param name="size">解像度</param>
		void Create(GraphicsDevice* pGraphicsDevice,const Math::Vector2& size);

		/// <summary>
		/// シェーダにセットする関数
		/// </summary>
		/// <param name="descIndex">レジスタ番号</param>
		void Set(int descIndex) override;
	};
}
