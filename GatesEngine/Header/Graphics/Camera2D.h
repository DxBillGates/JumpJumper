#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//2D�p�̃J������z�肵��Camera2D�N���X
	class Camera2D : public Camera
	{
	protected:
		Math::Vector2 size;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Camera2D();

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~Camera2D();

		/// <summary>
		/// GPU�ɃJ�����̃f�[�^���Z�b�g����֐�
		/// </summary>
		/// <param name="setDescriptorIndex">�Z�b�g���郌�W�X�^�ԍ�</param>
		void Set(int setDescriptorIndex) override;

		/// <summary>
		/// ���s���e�͈̔͂�ݒ肷��֐�
		/// </summary>
		/// <param name="s">�T�C�Y</param>
		void SetSize(const Math::Vector2& s);
	};

}