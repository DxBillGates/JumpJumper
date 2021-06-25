#pragma once
#include "GraphicsDevice.h"
namespace GatesEngine
{
	enum class RangeType
	{
		CBV,
		SRV,
		UAV
	};

	class RootSignature
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		std::vector<RangeType> rangeType;
		ID3DBlob* rootBlob;
		ID3D12RootSignature* rootSignature;
	private:
		void CreateRange(D3D12_DESCRIPTOR_RANGE& range, RangeType type, int count);
		void CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc);
	public:
		RootSignature(GraphicsDevice* graphicsDevice, const std::vector<RangeType>& rangeTypes);
		~RootSignature();
		void Create();
		ID3D12RootSignature* Get();
	};
}
