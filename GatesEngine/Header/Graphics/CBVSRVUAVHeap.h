#pragma once
#include "GraphicsDevice.h"

namespace GatesEngine
{
	class CBVSRVUAVHeap
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3D12DescriptorHeap* heap;
		Math::Vector3 useCount;
		bool isCreated;
		int incrementSize;
		int nextSrvDescriptorNum;
	public:
		CBVSRVUAVHeap();
		~CBVSRVUAVHeap();
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);
		void Create(const Math::Vector3& useCount);
		ID3D12DescriptorHeap* GetHeap();
		Math::Vector3 GetUseCount();
		void CreateSRV(ID3D12Resource* buffer);
		void CreateSRV(ID3D12Resource* buffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForSRV(int number);
		int GetNextSrvNumber();
		void Set();
	};
}
