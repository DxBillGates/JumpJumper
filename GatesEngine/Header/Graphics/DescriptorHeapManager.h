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
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="graphicsDevice">GraphicsDevice�̃|�C���^</param>
		/// <param name="cbvNum">CBV�Ƃ��Ĉ����鋖�e�l</param>
		/// <param name="srvNum">SRV�Ƃ��Ĉ����鋖�e�l</param>
		/// <param name="uavNum">UAV�Ƃ��Ĉ����鋖�e�l</param>
		DescriptorHeapManager(GraphicsDevice* graphicsDevice, int cbvNum, int srvNum, int uavNum = 0);
		~DescriptorHeapManager();

		/// <summary>
		/// �萔�o�b�t�@�r���[�̐����֐�
		/// </summary>
		/// <param name="cBuff">�萔�o�b�t�@</param>
		void CreateCBV(ID3D12Resource* cBuff);

		/// <summary>
		/// �V�F�[�_�[���\�[�X�r���[�̐����֐�
		/// </summary>
		/// <param name="texBuff">�e�N�X�`���o�b�t�@</param>
		void CreateSRV(ID3D12Resource* texBuff);

		/// <summary>
		/// DescriptorHeap�̃Z�b�g�֐�
		/// </summary>
		void Set();

		/// <summary>
		/// DescriptorHeap�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>DescriptorHeap�̃|�C���^</returns>
		ID3D12DescriptorHeap* Get();

		/// <summary>
		/// �����̔ԍ�����DescriptorHeap�̂ǂ��̔Ԓn�ɓo�^����Ă��邩�Ԃ��֐�(CBV:CPU��)
		/// </summary>
		/// <param name="number">�ԍ�</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCBVHandleForCPU(int number);

		/// <summary>
		/// �����̔ԍ�����DescriptorHeap�̂ǂ��̔Ԓn�ɓo�^����Ă��邩�Ԃ��֐�(CBV:GPU��)
		/// </summary>
		/// <param name="number">�ԍ�</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandleForGPU(int number);

		/// <summary>
		/// �����̔ԍ�����DescriptorHeap�̂ǂ��̔Ԓn�ɓo�^����Ă��邩�Ԃ��֐�(SRV:CPU��)
		/// </summary>
		/// <param name="number">�ԍ�</param>
		/// <returnescriptorHandle</returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandleForCPU(int number);

		/// <summary>
		/// �����̔ԍ�����DescriptorHeap�̂ǂ��̔Ԓn�ɓo�^����Ă��邩�Ԃ��֐�(SRVGPU��)
		/// </summary>
		/// <param name="number">�ԍ�</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForGPU(int number);

		/// <summary>
		/// ���ɓo�^�ł���CBV�p�̔ԍ���Ԃ��֐�
		/// </summary>
		/// <returns>���ɓo�^�ł���CBV�p�̔ԍ�</returns>
		int GetNextCbvDescriptorNumber();

		/// <summary>
		/// ���ɓo�^�ł���SRV�p�̔ԍ���Ԃ��֐�
		/// </summary>
		/// <returns>���ɓo�^�ł���SRV�p�̔ԍ�</returns>
		int GetNextSrvDescriptorNumber();
	};


}