#pragma once
#include "Component.h"
#include "..\Math\Math.h"
#include "..\Graphics\Mesh.h"
#include "..\Graphics\Shader.h"

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
		CollisionTreeObject* treeObj;
		Mesh* mesh;
		Shader* shader;
	public:
		Collider();
		~Collider();
		void Start() override;
		void OnDraw() override;
		void OnLateDraw() override;
		void SetType(ColliderType sType);
		ColliderType GetType();
		void SetSize(const Math::Vector3& sSize);
		Math::Vector3 GetSize();
		void SetCollisionTreeObject(CollisionTreeObject* obj);
		CollisionTreeObject* GetCollisionTreeObject();
	};
}
