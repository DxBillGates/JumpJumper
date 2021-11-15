#pragma once
#include "GraphicsDevice.h"
#include <d3d12.h>

namespace GatesEngine
{
	/// <summary>
	/// 深度バッファを扱うクラス
	/// </summary>
	class DepthStencil
	{
	protected:
		GraphicsDevice* graphicsDevice;
		ID3D12Resource* depthBuffer;
		ID3D12DescriptorHeap* heap;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DepthStencil();

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~DepthStencil();

		/// <summary>
		/// DSVとして扱うGPUバッファの生成関数
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
		/// <param name="size">解像度</param>
		void CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);

		/// <summary>
		/// セットされている深度バッファのポインタを返す関数
		/// </summary>
		/// <returns>セットされている深度バッファのポインタ</returns>
		ID3D12Resource* GetResource();

		/// <summary>
		/// セットされているヒープのポインタを返す関数
		/// </summary>
		/// <returns>セットされているヒープのポインタ</returns>
		ID3D12DescriptorHeap* GetHeap();

		/// <summary>
		/// 深度バッファに書き込まれているデータを初期化する関数
		/// </summary>
		void Clear();
	};
}
