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
		Collider();
		~Collider();
		void Start() override;
		void Update() override;
		void OnDraw() override;
		void OnLateDraw() override;
		void SetColor(const Math::Vector4& c);
		void SetType(ColliderType sType);
		void SetPosition(const Math::Vector3& pos);
		void SetTransform(const GatesEngine::Transform& transform);
		ColliderType GetType();
		void SetSize(const Math::Vector3& sSize);
		Math::Vector3 GetSize();
		void SetCollisionTreeObject(CollisionTreeObject* obj);
		CollisionTreeObject* GetCollisionTreeObject();
		GatesEngine::Transform* GetTransform();
	};
}
