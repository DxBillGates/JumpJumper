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

	enum class TextureAddressMode
	{
		WRAP,
		CLAMP
	};

	enum class D3D12Filter
	{
		POINT,
		LINEAR
	};

	class RootSignature
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		std::vector<RangeType> rangeType;
		ID3DBlob* rootBlob;
		ID3D12RootSignature* rootSignature;
	private:
		void CreateRange(D3D12_DESCRIPTOR_RANGE* range, RangeType type, int count);
		void CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc);
		void CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC* pSamplerDesc,TextureAddressMode mode,D3D12Filter filter,int count);
	public:
		RootSignature(GraphicsDevice* graphicsDevice, const std::vector<RangeType>& rangeTypes);
		~RootSignature();
		void Create();
		ID3D12RootSignature* Get();
	};
}
