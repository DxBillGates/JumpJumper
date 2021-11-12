#pragma once
#include "..\Component\Behaviour.h"
#include "..\Math\Transform.h"
#include "..\Component\Collider.h"
#include <vector>

namespace GatesEngine
{
	class GraphicsDevice;
	class GameObject
	{
	private:
		std::vector<Component*> components;
		std::vector<Behaviour*> behaviors;
		const char* name;
		const char* tag;
		GraphicsDevice* graphicsDevice;
		Transform transform;
		Collider* collider;
		bool enabled;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		GameObject();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="name">�Q�[���I�u�W�F�N�g�̖��O</param>
		/// <param name="tag">�Q�[���I�u�W�F�N�g�̃^�O</param>
		GameObject(const char* name, const char* tag = "none");
		~GameObject();

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g�����ׂď���������֐�
		/// </summary>
		void Start();

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g�����ׂčX�V����֐�
		/// </summary>
		void Update();

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g���ׂĂ̕`��֐����Ăяo���֐�
		/// </summary>
		void Draw();

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g���ׂĂ̒x���`��֐����Ăяo���֐�
		/// </summary>
		void LateDraw();

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g���ׂĂ�Collision���Ăяo���֐�
		/// </summary>
		/// <param name="other">�Փ˂����Q�[���I�u�W�F�N�g</param>
		void Collision(GameObject* other);

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g���ׂĂ�Collision���Ăяo���֐�
		/// </summary>
		/// <param name="other">�Փ˂����R���C�_�[</param>
		void Collision(Collider* hitCollider);

		/// <summary>
		/// �`��Ŏg��GraphicsDevice�̃Z�b�g������֐�
		/// </summary>
		/// <param name="pGraphicsDevice">�Z�b�g����GraphicsDevice�̃|�C���^</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// �ݒ肳��Ă���R���|�[�l���g�𑖍����ăR���C�_�[�R���|�[�l���g�����������ꍇ�͂���������o�ɃZ�b�g����
		/// </summary>
		void SetCollider();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̖��O��ύX����֐�
		/// </summary>
		/// <param name="sName">�Z�b�g���閼�O���</param>
		void SetName(const char* sName);

		/// <summary>
		/// �Q�[���I�u�W�F�N�g�̃^�O��ύX����֐�
		/// </summary>
		/// <param name="sTag">�Z�b�g����^�O</param>
		void SetTag(const char* sTag);

		/// <summary>
		/// �ݒ肳��Ă��閼�O����Ԃ��֐�
		/// </summary>
		/// <returns>�ݒ肳��Ă��閼�O���</returns>
		const char* GetName();

		/// <summary>
		/// �ݒ肳��Ă���^�O��Ԃ��֐�
		/// </summary>
		/// <returns>�ݒ肳��Ă���^�O</returns>
		const char* GetTag();

		/// <summary>
		/// �ݒ肳��Ă���GraphicsDevice�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�ݒ肳��Ă���GraphicsDevice�̃|�C���^</returns>
		GraphicsDevice* GetGraphicsDevice();

		/// <summary>
		/// �ʒu�A�p���A�傫�����擾����֐�
		/// </summary>
		/// <returns>�ʒu�A�p���A�傫����ێ�����N���X�̃|�C���^</returns>
		Transform* GetTransform();

		/// <summary>
		/// �ݒ肳��Ă���R���C�_�[�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�ݒ肳��Ă���R���C�_�[�̃|�C���^</returns>
		Collider* GetCollider();

		/// <summary>
		/// �X�V�A�`��A�����蔻����s�����ݒ肷��֐�
		/// </summary>
		/// <param name="flag">�ݒ肷��l</param>
		void SetEnabled(bool flag);

		/// <summary>
		/// �X�V�A�`��A�����蔻����s�����m�F����֐�
		/// </summary>
		/// <returns>����</returns>
		bool GetEnabled();

		/// <summary>
		/// �R���|�[�l���g��ǉ�����֐�
		/// </summary>
		/// <typeparam name="T">�R���|�[�l���g���p�������N���X</typeparam>
		/// <returns>�ǉ������R���|�[�l���g�̃|�C���^</returns>
		template <typename T>
		T* AddComponent();

		/// <summary>
		/// �r�w�C�r�A��ǉ�����֐�
		/// </summary>
		/// <typeparam name="T">�r�w�C�r�A���p�������N���X</typeparam>
		/// <returns>�ǉ������r�w�C�r�A�̃|�C���^</returns>
		template <typename T>
		T* AddBehavior();

		/// <summary>
		/// �R���|�[�l���g���p�������N���X�̌^���w�肵�Ă����ݒ肳��Ă���R���|�[�l���g���瑖�����ă|�C���^��Ԃ��֐�
		/// </summary>
		/// <typeparam name="T">�R���|�[�l���g���p�������N���X�̌^���</typeparam>
		/// <returns>�ێ����Ă���Ύw�肵���^�̃|�C���^�A�ێ����Ă��Ȃ��ꍇ��nullptr</returns>
		template <typename T>
		T* GetComponent();
	};
	template<typename T>
	inline T* GameObject::AddComponent()
	{
		T* t = new T();
		//Component�N���X����h���������̂��`�F�b�N
		Component* pComponent = static_cast<Component*>(t);
		if (pComponent != nullptr)
		{
			components.push_back(pComponent);
			pComponent->SetGameObject(this);
			return t;
		}
		return nullptr;
	}
	template<typename T>
	inline T* GameObject::AddBehavior()
	{
		T* t = new T();
		//Component�N���X����h���������̂��`�F�b�N
		Behaviour* pComponent = static_cast<Behaviour*>(t);
		if (pComponent != nullptr)
		{
			behaviors.push_back(pComponent);
			pComponent->SetGameObject(this);
			return t;
		}
		return nullptr;
	}
	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (int i = 0; i < (int)components.size(); i++)
		{
			T* component = dynamic_cast<T*>(components[i]);
			if (component != nullptr)
			{
				return component;
			}
		}
		for (int i = 0; i < (int)behaviors.size(); ++i)
		{
			T* component = dynamic_cast<T*>(behaviors[i]);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
}
