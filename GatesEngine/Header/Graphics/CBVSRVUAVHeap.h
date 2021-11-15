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
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		CBVSRVUAVHeap();
		~CBVSRVUAVHeap();

		/// <summary>
		/// GraphicsDevice�̃|�C���^���Z�b�g����֐�
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// �q�[�v�̐����֐�
		/// </summary>
		/// <param name="useCount">CBV�ASRV�AUAV�̏��Ԃł����̃f�[�^���i�[�ł���悤�ɂ��邩�ݒ肷��l</param>
		void Create(const Math::Vector3& useCount);

		/// <summary>
		/// �f�B�X�N���v�^�q�[�v�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�f�B�X�N���v�^�q�[�v�̃|�C���^</returns>
		ID3D12DescriptorHeap* GetHeap();

		/// <summary>
		/// CBV�ASRV�AUAV�̏��Ԃł����̃f�[�^���i�[�ł��邩�Ԃ��֐�
		/// </summary>
		/// <returns>�e�f�[�^�̋��e�l</returns>
		Math::Vector3 GetUseCount();

		/// <summary>
		/// �����œn���ꂽGPU�o�b�t�@����ShaderResourceView�𐶐�����֐�
		/// </summary>
		/// <param name="buffer">GPUBuffer</param>
		void CreateSRV(ID3D12Resource* buffer);

		/// <summary>
		/// �����œn���ꂽGPU�o�b�t�@�Ǝw�肳�ꂽView�̐����f�[�^����ShaderResourceView�𐶐�����֐�
		/// </summary>
		/// <param name="buffer">GPUBuffer</param>
		/// <param name="viewDesc"></param>
		void CreateSRV(ID3D12Resource* buffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

		/// <summary>
		/// SRV��GPU���A�h���X��Ԃ��֐�
		/// </summary>
		/// <param name="number">�e�N�X�`���ԍ�</param>
		/// <returns>SRV��GPU���A�h���X</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForSRV(int number);

		/// <summary>
		/// ���ɐ��������ׂ�SRV�p�̃f�B�X�N���v�^�q�[�v�̔Ԓn��Ԃ��֐�
		/// </summary>
		/// <returns>���ɐ��������ׂ�SRV�p�̃f�B�X�N���v�^�q�[�v�̔Ԓn</returns>
		int GetNextSrvNumber();

		/// <summary>
		/// �q�[�v���Z�b�g����֐�
		/// </summary>
		void Set();
	};
}
