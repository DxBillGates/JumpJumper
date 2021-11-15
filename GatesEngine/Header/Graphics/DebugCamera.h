#pragma once
#include "Camera3D.h"

namespace GatesEngine
{
	class Input;
	/// <summary>
	/// �f�o�b�O���Ɏg�����R�ړ��\�ȃJ������z��Ƃ����N���X�A�}�E�X���g���ăJ�����̎��_�ړ���A
	/// �L�[�{�[�h���g�p���Ĉړ����\
	/// </summary>
	class DebugCamera : public Camera3D
	{
	protected:
		Input* input;
		float moveSpeed;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		DebugCamera();

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~DebugCamera();

		/// <summary>
		/// �������֐�
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// �X�V�֐�
		/// </summary>
		void Update() override;
	};
}
