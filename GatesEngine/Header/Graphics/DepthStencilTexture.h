#pragma once
#include "GraphicsDevice.h"
#include "Texture.h"
#include "DepthStencil.h"

namespace GatesEngine
{
	class DepthStencilTexture : public Texture,public DepthStencil
	{
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		DepthStencilTexture();
		~DepthStencilTexture();

		/// <summary>
		/// �[�x�e�N�X�`�������֐�
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		/// <param name="size">�𑜓x</param>
		void Create(GraphicsDevice* pGraphicsDevice,const Math::Vector2& size);

		/// <summary>
		/// �V�F�[�_�ɃZ�b�g����֐�
		/// </summary>
		/// <param name="descIndex">���W�X�^�ԍ�</param>
		void Set(int descIndex) override;
	};
}
