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
		/// コンストラクタ
		/// </summary>
		Collider();

		~Collider();

		/// <summary>
		///　初期化関数のオーバーライド
		/// </summary>
		void Start() override;

		/// <summary>
		/// 更新関数のオーバーライド
		/// </summary>
		void Update() override;

		/// <summary>
		/// 描画関数のオーバーライド
		/// </summary>
		void OnDraw() override;

		/// <summary>
		/// 遅延描画関数のオーバーライド
		/// </summary>
		void OnLateDraw() override;

		/// <summary>
		/// コライダーの色を設定する関数
		/// </summary>
		/// <param name="c">設定する色情報</param>
		void SetColor(const Math::Vector4& c);

		/// <summary>
		/// コライダーの種類を設定する関数
		/// </summary>
		/// <param name="sType">設定するコライダーの種類</param>
		void SetType(ColliderType sType);

		/// <summary>
		/// コライダーの中心座標を設定する関数、GameObjectと相対位置
		/// </summary>
		/// <param name="pos">GameObjectと相対位置に設定する値</param>
		void SetPosition(const Math::Vector3& pos);

		/// <summary>
		/// コライダーの見た目上の姿勢や位置、大きさを設定する関数
		/// </summary>
		/// <param name="transform">設定する値</param>
		void SetTransform(const GatesEngine::Transform& transform);

		/// <summary>
		/// 設定されているコライダーの種類を返す
		/// </summary>
		/// <returns>設定されているコライダーの種類</returns>
		ColliderType GetType();

		/// <summary>
		/// コライダーの大きさを設定する関数、GameObjectの大きさと相対関係
		/// </summary>
		/// <param name="sSize">設定する大きさ</param>
		void SetSize(const Math::Vector3& sSize);

		/// <summary>
		/// コライダーの大きさを返す関数
		/// </summary>
		/// <returns>コライダーの大きさ</returns>
		Math::Vector3 GetSize();

		void SetCollisionTreeObject(CollisionTreeObject* obj);
		CollisionTreeObject* GetCollisionTreeObject();
		GatesEngine::Transform* GetTransform();
	};
}
