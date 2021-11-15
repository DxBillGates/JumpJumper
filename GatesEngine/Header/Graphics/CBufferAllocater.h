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

		/// <summary>
		/// GraphicsDevice�̃|�C���^���Z�b�g����֐�
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// �o�J�ł����f�B�X�N���v�^�q�[�v�̃|�C���^���Z�b�g����֐�
		/// </summary>
		/// <param name="pHeap">�q�[�v�̃|�C���^</param>
		void SetHeap(CBVSRVUAVHeap* pHeap);

		/// <summary>
		/// �A���P�[�^�̐����֐�
		/// </summary>
		void Create();

		/// <summary>
		/// �g�p���Ă���o�b�t�@�̔ԍ���������������֐�
		/// </summary>
		void ResetCurrentUseNumber();

		/// <summary>
		/// �Z�b�g���郌�W�X�^�ԍ��A�Z�b�g����f�[�^�̃|�C���^�A�f�[�^�T�C�Y���w�肵��
		/// ���ꂩ��g���萔�o�b�t�@�̔Ԓn�̌v�Z���s�����̂��f�[�^��
		/// �V�F�[�_�Ƀf�[�^���Z�b�g����֐��A��ɔz��f�[�^���Z�b�g�������Ƃ��Ɏg�p
		/// </summary>
		/// <param name="descIndex">���W�X�^�ԍ�</param>
		/// <param name="data">�f�[�^�|�C���^</param>
		/// <param name="size">�f�[�^�T�C�Y</param>
		void BindAndAttachData(int descIndex, const void* data, int size);

		/// <summary>
		/// �Z�b�g���郌�W�X�^�ԍ��A�f�[�^���w�肵�Ă��ꂩ��g���萔�o�b�t�@�̔Ԓn�̌v�Z���s�����̂��f�[�^���V�F�[�_�ɃZ�b�g����֐�
		/// </summary>
		/// <typeparam name="T">�Z�b�g����f�[�^�̌^</typeparam>
		/// <param name="descIndex">���W�X�^�ԍ�</param>
		/// <param name="data">�f�[�^</param>
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