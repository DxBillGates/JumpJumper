#pragma once
#include "GraphicsDevice.h"
#include "CBVSRVUAVHeap.h"

namespace GatesEngine
{
	class CBufferAllocater
	{
	private:
		GraphicsDevice* graphicsDevice;
		CBVSRVUAVHeap* heap;
		ID3D12Resource* buffer;
		struct { char buf[256]; }*mappedBuffer;
		int currentUseNumber;
		bool isCreated;
	public:
		~CBufferAllocater();
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);
		void SetHeap(CBVSRVUAVHeap* pHeap);
		void Create();
		void ResetCurrentUseNumber();
		void BindAndAttachData(int descIndex, const void* data, int size);
		template<typename T>
		void BindAndAttach(int descIndex,const T& data);
	};
	template<typename T>
	inline void CBufferAllocater::BindAndAttach(int descIndex, const T& data)
	{
		if (!heap)return;

		//dataのサイズを256アライメントして計算
		int sizeAligned = (sizeof(T) + 0xff) & ~0xff;
		//256byteをいくつ使用するかアライメントした結果を256で割って計算
		int useValue = sizeAligned / 0x100;

		//現在使われ終わってる番号と今から使う容量がヒープの容量を超えている場合はリターン
		if (currentUseNumber + useValue > (int)heap->GetUseCount().x)return;

		int top = currentUseNumber;

		//先頭アドレスに使う分のポインタを足してmemcpy
		memcpy(mappedBuffer + top, &data, sizeof(T));

		//ビューを作って値をShaderにアタッチ
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
		cbDesc.BufferLocation = buffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
		cbDesc.SizeInBytes = sizeAligned;

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;
		graphicsDevice->GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
		gpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;

		graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

		currentUseNumber += useValue;
	}
}