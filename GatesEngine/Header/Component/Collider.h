#pragma once
#include "Component.h"
#include "..\Math\Math.h"
#include "..\Graphics\Mesh.h"
#include "..\Graphics\Shader.h"
#include "..\Math\Transform.h"

namespace GatesEngine
{
	enum class ColliderType
	{
		CUBE,
		SPHERE,
	};
	class CollisionTreeObject;
	class Collider : public Component
	{
	private:
		ColliderType type;
		Math::Vector3 size;
		Math::Vector4 color;
		GatesEngine::Transform transform;
		CollisionTreeObject* treeObj;
		Mesh* mesh;
		Shader* shader;
		bool isOldHit;
		int hitFrameCount;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Collider();

		~Collider();

		/// <summary>
		///�@�������֐��̃I�[�o�[���C�h
		/// </summary>
		void Start() override;

		/// <summary>
		/// �X�V�֐��̃I�[�o�[���C�h
		/// </summary>
		void Update() override;

		/// <summary>
		/// �`��֐��̃I�[�o�[���C�h
		/// </summary>
		void OnDraw() override;

		/// <summary>
		/// �x���`��֐��̃I�[�o�[���C�h
		/// </summary>
		void OnLateDraw() override;

		/// <summary>
		/// �R���C�_�[�̐F��ݒ肷��֐�
		/// </summary>
		/// <param name="c">�ݒ肷��F���</param>
		void SetColor(const Math::Vector4& c);

		/// <summary>
		/// �R���C�_�[�̎�ނ�ݒ肷��֐�
		/// </summary>
		/// <param name="sType">�ݒ肷��R���C�_�[�̎��</param>
		void SetType(ColliderType sType);

		/// <summary>
		/// �R���C�_�[�̒��S���W��ݒ肷��֐��AGameObject�Ƒ��Έʒu
		/// </summary>
		/// <param name="pos">GameObject�Ƒ��Έʒu�ɐݒ肷��l</param>
		void SetPosition(const Math::Vector3& pos);

		/// <summary>
		/// �R���C�_�[�̌����ڏ�̎p����ʒu�A�傫����ݒ肷��֐�
		/// </summary>
		/// <param name="transform">�ݒ肷��l</param>
		void SetTransform(const GatesEngine::Transform& transform);

		/// <summary>
		/// �ݒ肳��Ă���R���C�_�[�̎�ނ�Ԃ�
		/// </summary>
		/// <returns>�ݒ肳��Ă���R���C�_�[�̎��</returns>
		ColliderType GetType();

		/// <summary>
		/// �R���C�_�[�̑傫����ݒ肷��֐��AGameObject�̑傫���Ƒ��Ί֌W
		/// </summary>
		/// <param name="sSize">�ݒ肷��傫��</param>
		void SetSize(const Math::Vector3& sSize);

		/// <summary>
		/// �R���C�_�[�̑傫����Ԃ��֐�
		/// </summary>
		/// <returns>�R���C�_�[�̑傫��</returns>
		Math::Vector3 GetSize();

		void SetCollisionTreeObject(CollisionTreeObject* obj);
		CollisionTreeObject* GetCollisionTreeObject();
		GatesEngine::Transform* GetTransform();
	};
}
