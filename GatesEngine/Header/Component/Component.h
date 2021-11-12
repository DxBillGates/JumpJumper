#pragma once

namespace GatesEngine
{
	class GameObject;
	class Collider;
	class Component
	{
	private:
	protected:
		const char* name;
		const char* tag;
		bool isEnabled;
		GameObject* gameObject;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Component();

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~Component();

		/// <summary>
		/// �V�[����ύX�����Ƃ��Ɉ�x�����Ă΂�鏉�����֐�
		/// </summary>
		virtual void Start();

		/// <summary>
		/// ���t���[���Ă΂��X�V�֐�
		/// </summary>
		virtual void Update();

		/// <summary>
		/// ���t���[���Ă΂��`��֐�
		/// </summary>
		virtual void OnDraw();

		/// <summary>
		/// ���ׂĂ�OnDraw�֐����Ă΂ꂽ��ɌĂ΂��x���`��֐�
		/// </summary>
		virtual void OnLateDraw();

		/// <summary>
		/// ���̃Q�[���I�u�W�F�N�g�ƃQ�[���I�u�W�F�N�g���Փ˂����ۂɌĂ΂��֐�
		/// </summary>
		/// <param name="other">�Փ˂����Q�[���I�u�W�F�N�g</param>
		virtual void OnCollision(GameObject* other);

		/// <summary>
		/// ���̃Q�[���I�u�W�F�N�g�ƃQ�[���I�u�W�F�N�g���Փ˂����ۂɌĂ΂��֐�
		/// </summary>
		/// <param name="hitCollider">�Փ˂����R���C�_�[</param>
		virtual void OnCollision(Collider* hitCollider);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g��ݒ肷��֐�
		/// </summary>
		/// <param name="gameObject">�ݒ肷��Q�[���I�u�W�F�N�g�̃|�C���^</param>
		void SetGameObject(GameObject* gameObject);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O�ƃ^�O��ݒ肷��֐�
		/// </summary>
		/// <param name="name">�ݒ肷�閼�O</param>
		/// <param name="tag">�ݒ肷��^�O</param>
		void SetNameAndTag(const char* name, const char* tag);

		/// <summary>
		/// �X�V�A�`��A�����蔻������邩�ݒ肷��֐�
		/// </summary>
		/// <param name="flag">�ݒ肷��l</param>
		void SetEnabled(bool flag);

		/// <summary>
		/// �X�V�A�`��A�����蔻�肪����Ă��邩��Ԃ��֐�
		/// </summary>
		/// <returns>����</returns>
		bool GetEnabled();

		/// <summary>
		/// �ݒ肳��Ă���Q�[���I�u�W�F�N�g�̃|�C���^��Ԃ�
		/// </summary>
		/// <returns>�Q�[���I�u�W�F�N�g�̃|�C���^</returns>
		GameObject* GetGameObject();
	};
}
