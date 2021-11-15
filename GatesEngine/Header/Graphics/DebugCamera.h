#pragma once
#include "Camera3D.h"

namespace GatesEngine
{
	class Input;
	/// <summary>
	/// デバッグ中に使う自由移動可能なカメラを想定としたクラス、マウスを使ってカメラの視点移動や、
	/// キーボードを使用して移動が可能
	/// </summary>
	class DebugCamera : public Camera3D
	{
	protected:
		Input* input;
		float moveSpeed;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		DebugCamera();

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~DebugCamera();

		/// <summary>
		/// 初期化関数
		/// </summary>
		void Initialize() override;

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update() override;
	};
}
