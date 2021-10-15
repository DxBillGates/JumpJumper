#pragma once
#include "RootSignature.h"
#include <d3d12.h>
#include <string>
#include <vector>

namespace GatesEngine
{
	class ComputePipeline
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3DBlob* shaderBlob;
		ID3D12PipelineState* pipeline;
		RootSignature* rootSignature;
	public:
		ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filename);
		~ComputePipeline();
		void Create(const std::vector<RangeType>& range);
		void Set();
	};
}
