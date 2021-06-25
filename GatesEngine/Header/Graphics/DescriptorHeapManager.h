#pragma once
#include "GraphicsDevice.h"
#include<d3d12.h>
namespace GatesEngine
{
	class DescriptorHeapManager
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		ID3D12DescriptorHeap* mHeap;
		int srvStartOffset;
		int incrementSize;
		int nextCbvDescriptorNum;
		int nextSrvDescriptorNum;
	public:
		DescriptorHeapManager(GraphicsDevice* graphicsDevice, int cbvNum, int srvNum, int uavNum = 0);
		~DescriptorHeapManager();
		void CreateCBV(ID3D12Resource* cBuff);
		void CreateSRV(ID3D12Resource* texBuff);
		void Set();
		ID3D12DescriptorHeap* Get();
		D3D12_CPU_DESCRIPTOR_HANDLE GetCBVHandleForCPU(int number);
		D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandleForGPU(int number);
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandleForCPU(int number);
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForGPU(int number);
		int GetNextCbvDescriptorNumber();
		int GetNextSrvDescriptorNumber();
	};


}