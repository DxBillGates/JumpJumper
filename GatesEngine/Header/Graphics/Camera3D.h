#pragma once
#include "Camera.h"

namespace GatesEngine
{
	//3DCameraを想定としたクラス、Setメンバによるカメラ操作が可能
	class Camera3D : public Camera
	{
	protected:
		Math::Matrix4x4 viewMatrix;

		Math::Vector3 position;
		Math::Vector3 direction;
		Math::Vector3 up;

		float yaw;
		float pitch;

		Math::Matrix4x4 rotation;

		Math::Vector3 addVector;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Camera3D();

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~Camera3D();

		/// <summary>
		/// 更新関数
		/// </summary>
		void Update() override;

		/// <summary>
		/// GPUにカメラデータをセットする関数
		/// </summary>
		/// <param name="setDescriptorIndex">セットするレジスタ番号</param>
		void Set(int setDescriptorIndex) override;

		/// <summary>
		/// カメラの位置を設定する関数
		/// </summary>
		/// <param name="pos">位置</param>
		void SetPosition(const Math::Vector3& pos);

		/// <summary>
		/// カメラの向きを設定する関数
		/// </summary>
		/// <param name="dir">方向</param>
		void SetDirection(const Math::Vector3& dir);

		/// <summary>
		/// カメラのY軸回転を制御する関数
		/// </summary>
		/// <param name="value">値</param>
		void SetYaw(float value);

		/// <summary>
		/// カメラのX軸回転を制御する関数
		/// </summary>
		/// <param name="value">値</param>
		void SetPitch(float value);

		/// <summary>
		/// カメラの回転行列を参照渡しで返す関数
		/// </summary>
		/// <returns>カメラの回転行列</returns>
		const Math::Matrix4x4& GetRotation();

		/// <summary>
		/// カメラの位置を返す関数
		/// </summary>
		/// <returns>カメラの位置</returns>
		const Math::Vector3& GetPosition();
	};
}
