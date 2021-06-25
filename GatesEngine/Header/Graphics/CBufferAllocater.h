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

		//data�̃T�C�Y��256�A���C�����g���Čv�Z
		int sizeAligned = (sizeof(T) + 0xff) & ~0xff;
		//256byte�������g�p���邩�A���C�����g�������ʂ�256�Ŋ����Čv�Z
		int useValue = sizeAligned / 0x100;

		//���ݎg���I����Ă�ԍ��ƍ�����g���e�ʂ��q�[�v�̗e�ʂ𒴂��Ă���ꍇ�̓��^�[��
		if (currentUseNumber + useValue > (int)heap->GetUseCount().x)return;

		int top = currentUseNumber;

		//�擪�A�h���X�Ɏg�����̃|�C���^�𑫂���memcpy
		memcpy(mappedBuffer + top, &data, sizeof(T));

		//�r���[������Ēl��Shader�ɃA�^�b�`
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