#pragma once
#include "GraphicsDevice.h"
#include <d3d12.h>

namespace GatesEngine
{
	/// <summary>
	/// �[�x�o�b�t�@�������N���X
	/// </summary>
	class DepthStencil
	{
	protected:
		GraphicsDevice* graphicsDevice;
		ID3D12Resource* depthBuffer;
		ID3D12DescriptorHeap* heap;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		DepthStencil();

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~DepthStencil();

		/// <summary>
		/// DSV�Ƃ��Ĉ���GPU�o�b�t�@�̐����֐�
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		/// <param name="size">�𑜓x</param>
		void CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size);

		/// <summary>
		/// �Z�b�g����Ă���[�x�o�b�t�@�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�Z�b�g����Ă���[�x�o�b�t�@�̃|�C���^</returns>
		ID3D12Resource* GetResource();

		/// <summary>
		/// �Z�b�g����Ă���q�[�v�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�Z�b�g����Ă���q�[�v�̃|�C���^</returns>
		ID3D12DescriptorHeap* GetHeap();

		/// <summary>
		/// �[�x�o�b�t�@�ɏ������܂�Ă���f�[�^������������֐�
		/// </summary>
		void Clear();
	};
}
