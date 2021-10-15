#pragma once
#include "..\..\Math\Math.h"
#include "..\GraphicsDevice.h"
#include <d3d12.h>

namespace GatesEngine
{
	struct ParticleData
	{
		Math::Vector4 pos;
		Math::Vector4 vel;
	};

	class GPUParticleManager
	{
	private:
		const UINT maxParticleValue;
		UINT nextParticleOffset;
		UINT maxDescriptorValue;
		UINT uavNextOffset;
		UINT srvNextOffset;

		ID3D12Resource* particleBuffer;
		ID3D12Resource* updateParticleBuffer;
		ID3D12DescriptorHeap* heap;

		GraphicsDevice* graphicsDevice;
	public:
		GPUParticleManager(GraphicsDevice* pGraphicsDevice);
		~GPUParticleManager();
		UINT GetMaxParticleValue();
		UINT GetNextParticleOffset();
		UINT GetUavNextOffset();
		UINT GetSrvNextOffset();
		ID3D12Resource* GetParticleBuffer();
		ID3D12Resource* GetUpdateParticleBuffer();
		ID3D12DescriptorHeap* GetHeap();

		void IncrementParticleOffset(UINT value);
		void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc);
		void CreateSRV(ID3D12Resource* buffer,const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

		GraphicsDevice* GetDevice();
	};
}