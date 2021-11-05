#pragma once
#include "GraphicsDevice.h"

namespace GatesEngine
{
	class SamplerHeap
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3D12DescriptorHeap* heap;
		Math::Vector3 useCount;
		bool isCreated;
		int incrementSize;
	private:
		void CreateSampler(const D3D12_SAMPLER_DESC& samplderDesc,int count);
	public:
		SamplerHeap();
		~SamplerHeap();
		void SetGraphicsDevice(GraphicsDevice* gDevice);
		void Create();
	};
}
