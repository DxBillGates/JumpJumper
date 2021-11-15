#pragma once
#include "RootSignature.h"
#include <d3d12.h>
#include <string>
#include <vector>

namespace GatesEngine
{
	/// <summary>
	/// コンピュートシェーダを扱えるようにする関数
	/// </summary>
	class ComputePipeline
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3DBlob* shaderBlob;
		ID3D12PipelineState* pipeline;
		RootSignature* rootSignature;
	public:
		/// <summary>
		/// コンストラクタ、兼シェーダーコンパイル
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
		/// <param name="filename">コンパイルするシェーダーファイルの名前</param>
		ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filename);
		~ComputePipeline();

		/// <summary>
		/// 生成関数
		/// </summary>
		/// <param name="range">シェーダーで扱うRootParamの配列</param>
		void Create(const std::vector<RangeType>& range);

		/// <summary>
		/// シェーダーをセットする関数
		/// </summary>
		void Set();
	};
}
