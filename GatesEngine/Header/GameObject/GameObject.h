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
		/// コンストラクタ
		/// </summary>
		GameObject();

		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="name">ゲームオブジェクトの名前</param>
		/// <param name="tag">ゲームオブジェクトのタグ</param>
		GameObject(const char* name, const char* tag = "none");
		~GameObject();

		/// <summary>
		/// 設定されているコンポーネントをすべて初期化する関数
		/// </summary>
		void Start();

		/// <summary>
		/// 設定されているコンポーネントをすべて更新する関数
		/// </summary>
		void Update();

		/// <summary>
		/// 設定されているコンポーネントすべての描画関数を呼び出す関数
		/// </summary>
		void Draw();

		/// <summary>
		/// 設定されているコンポーネントすべての遅延描画関数を呼び出す関数
		/// </summary>
		void LateDraw();

		/// <summary>
		/// 設定されているコンポーネントすべてのCollisionを呼び出す関数
		/// </summary>
		/// <param name="other">衝突したゲームオブジェクト</param>
		void Collision(GameObject* other);

		/// <summary>
		/// 設定されているコンポーネントすべてのCollisionを呼び出す関数
		/// </summary>
		/// <param name="other">衝突したコライダー</param>
		void Collision(Collider* hitCollider);

		/// <summary>
		/// 描画で使うGraphicsDeviceのセットをする関数
		/// </summary>
		/// <param name="pGraphicsDevice">セットするGraphicsDeviceのポインタ</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// 設定されているコンポーネントを走査してコライダーコンポーネントが見つかった場合はそれをメンバにセットする
		/// </summary>
		void SetCollider();

		/// <summary>
		/// ゲームオブジェクトの名前を変更する関数
		/// </summary>
		/// <param name="sName">セットする名前情報</param>
		void SetName(const char* sName);

		/// <summary>
		/// ゲームオブジェクトのタグを変更する関数
		/// </summary>
		/// <param name="sTag">セットするタグ</param>
		void SetTag(const char* sTag);

		/// <summary>
		/// 設定されている名前情報を返す関数
		/// </summary>
		/// <returns>設定されている名前情報</returns>
		const char* GetName();

		/// <summary>
		/// 設定されているタグを返す関数
		/// </summary>
		/// <returns>設定されているタグ</returns>
		const char* GetTag();

		/// <summary>
		/// 設定されているGraphicsDeviceのポインタを返す関数
		/// </summary>
		/// <returns>設定されているGraphicsDeviceのポインタ</returns>
		GraphicsDevice* GetGraphicsDevice();

		/// <summary>
		/// 位置、姿勢、大きさを取得する関数
		/// </summary>
		/// <returns>位置、姿勢、大きさを保持するクラスのポインタ</returns>
		Transform* GetTransform();

		/// <summary>
		/// 設定されているコライダーのポインタを返す関数
		/// </summary>
		/// <returns>設定されているコライダーのポインタ</returns>
		Collider* GetCollider();

		/// <summary>
		/// 更新、描画、当たり判定を行うか設定する関数
		/// </summary>
		/// <param name="flag">設定する値</param>
		void SetEnabled(bool flag);

		/// <summary>
		/// 更新、描画、当たり判定を行うか確認する関数
		/// </summary>
		/// <returns>成否</returns>
		bool GetEnabled();

		/// <summary>
		/// コンポーネントを追加する関数
		/// </summary>
		/// <typeparam name="T">コンポーネントを継承したクラス</typeparam>
		/// <returns>追加したコンポーネントのポインタ</returns>
		template <typename T>
		T* AddComponent();

		/// <summary>
		/// ビヘイビアを追加する関数
		/// </summary>
		/// <typeparam name="T">ビヘイビアを継承したクラス</typeparam>
		/// <returns>追加したビヘイビアのポインタ</returns>
		template <typename T>
		T* AddBehavior();

		/// <summary>
		/// コンポーネントを継承したクラスの型を指定してそれを設定されているコンポーネントから走査してポインタを返す関数
		/// </summary>
		/// <typeparam name="T">コンポーネントを継承したクラスの型情報</typeparam>
		/// <returns>保持していれば指定した型のポインタ、保持していない場合はnullptr</returns>
		template <typename T>
		T* GetComponent();
	};
	template<typename T>
	inline T* GameObject::AddComponent()
	{
		T* t = new T();
		//Componentクラスから派生したものかチェック
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
		//Componentクラスから派生したものかチェック
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
