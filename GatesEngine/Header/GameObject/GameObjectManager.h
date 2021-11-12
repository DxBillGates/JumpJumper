#pragma once
#include "GameObject.h"
namespace GatesEngine
{
	class GameObjectManager
	{
	private:
		std::vector<GameObject*> gameObjects;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GameObjectManager();
		~GameObjectManager();

		/// <summary>
		/// ゲームオブジェクトを追加する関数
		/// </summary>
		/// <param name="newGameObject">追加するゲームオブジェクトのポインタ</param>
		/// <returns>追加したゲームオブジェクトのポインタ</returns>
		GameObject* Add(GameObject* newGameObject);

		/// <summary>
		/// 名前情報、タグを指定してゲームオブジェクトが存在するか走査してそのゲームオブジェクトのポインタを返す関数
		/// </summary>
		/// <param name="name">指定する名前情報</param>
		/// <param name="tag">指定するタグ</param>
		/// <returns>走査して見つかったらそのゲームオブジェクトのポインタ、見つからなかった場合はnullptr</returns>
		GameObject* Find(const char* name, const char* tag = nullptr);

		/// <summary>
		/// 登録されているゲームオブジェクトの初期化をする関数
		/// </summary>
		void Start();

		/// <summary>
		/// 登録されているゲームオブジェクトの更新をする関数
		/// </summary>
		void Update();

		/// <summary>
		/// 登録されているゲームオブジェクトの描画をする関数
		/// </summary>
		void Draw();

		/// <summary>
		/// 登録されているゲームオブジェクトの遅延描画をする関数
		/// </summary>
		void LateDraw();
	};
}
