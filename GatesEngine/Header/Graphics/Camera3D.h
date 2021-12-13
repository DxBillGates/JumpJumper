#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//3DCamera��z��Ƃ����N���X�ASet�����o�ɂ��J�������삪�\
	class Camera3D : public Camera
	{
	protected:
		Math::Matrix4x4 viewMatrix;

		Math::Vector3 position;
		Math::Vector3 direction;
		Math::Vector3 up;

		float yaw;
		float pitch;

		Math::Matrix4x4 rotation;

		Math::Vector3 addVector;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Camera3D();

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~Camera3D();

		/// <summary>
		/// �X�V�֐�
		/// </summary>
		void Update() override;

		/// <summary>
		/// GPU�ɃJ�����f�[�^���Z�b�g����֐�
		/// </summary>
		/// <param name="setDescriptorIndex">�Z�b�g���郌�W�X�^�ԍ�</param>
		void Set(int setDescriptorIndex) override;

		/// <summary>
		/// �J�����̈ʒu��ݒ肷��֐�
		/// </summary>
		/// <param name="pos">�ʒu</param>
		void SetPosition(const Math::Vector3& pos);

		/// <summary>
		/// �J�����̌�����ݒ肷��֐�
		/// </summary>
		/// <param name="dir">����</param>
		void SetDirection(const Math::Vector3& dir);

		/// <summary>
		/// �J������Y����]�𐧌䂷��֐�
		/// </summary>
		/// <param name="value">�l</param>
		void SetYaw(float value);

		/// <summary>
		/// �J������X����]�𐧌䂷��֐�
		/// </summary>
		/// <param name="value">�l</param>
		void SetPitch(float value);

		/// <summary>
		/// �J�����̉�]�s����Q�Ɠn���ŕԂ��֐�
		/// </summary>
		/// <returns>�J�����̉�]�s��</returns>
		const Math::Matrix4x4& GetRotation();

		/// <summary>
		/// �J�����̈ʒu��Ԃ��֐�
		/// </summary>
		/// <returns>�J�����̈ʒu</returns>
		const Math::Vector3& GetPosition();
	};
}
