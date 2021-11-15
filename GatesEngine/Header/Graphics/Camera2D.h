#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//2D用のカメラを想定したCamera2Dクラス
	class Camera2D : public Camera
	{
	protected:
		Math::Vector2 size;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Camera2D();

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~Camera2D();

		/// <summary>
		/// GPUにカメラのデータをセットする関数
		/// </summary>
		/// <param name="setDescriptorIndex">セットするレジスタ番号</param>
		void Set(int setDescriptorIndex) override;

		/// <summary>
		/// 平行投影の範囲を設定する関数
		/// </summary>
		/// <param name="s">サイズ</param>
		void SetSize(const Math::Vector2& s);
	};

}