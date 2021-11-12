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
		/// コンストラクタ
		/// </summary>
		Component();

		/// <summary>
		/// デストラクタ
		/// </summary>
		virtual ~Component();

		/// <summary>
		/// シーンを変更したときに一度だけ呼ばれる初期化関数
		/// </summary>
		virtual void Start();

		/// <summary>
		/// 毎フレーム呼ばれる更新関数
		/// </summary>
		virtual void Update();

		/// <summary>
		/// 毎フレーム呼ばれる描画関数
		/// </summary>
		virtual void OnDraw();

		/// <summary>
		/// すべてのOnDraw関数が呼ばれた後に呼ばれる遅延描画関数
		/// </summary>
		virtual void OnLateDraw();

		/// <summary>
		/// 他のゲームオブジェクトとゲームオブジェクトが衝突した際に呼ばれる関数
		/// </summary>
		/// <param name="other">衝突したゲームオブジェクト</param>
		virtual void OnCollision(GameObject* other);

		/// <summary>
		/// 他のゲームオブジェクトとゲームオブジェクトが衝突した際に呼ばれる関数
		/// </summary>
		/// <param name="hitCollider">衝突したコライダー</param>
		virtual void OnCollision(Collider* hitCollider);

		/// <summary>
		/// ゲームオブジェクトを設定する関数
		/// </summary>
		/// <param name="gameObject">設定するゲームオブジェクトのポインタ</param>
		void SetGameObject(GameObject* gameObject);

		/// <summary>
		/// ゲームオブジェクトの名前とタグを設定する関数
		/// </summary>
		/// <param name="name">設定する名前</param>
		/// <param name="tag">設定するタグ</param>
		void SetNameAndTag(const char* name, const char* tag);

		/// <summary>
		/// 更新、描画、当たり判定をするか設定する関数
		/// </summary>
		/// <param name="flag">設定する値</param>
		void SetEnabled(bool flag);

		/// <summary>
		/// 更新、描画、当たり判定がされているかを返す関数
		/// </summary>
		/// <returns>成否</returns>
		bool GetEnabled();

		/// <summary>
		/// 設定されているゲームオブジェクトのポインタを返す
		/// </summary>
		/// <returns>ゲームオブジェクトのポインタ</returns>
		GameObject* GetGameObject();
	};
}
