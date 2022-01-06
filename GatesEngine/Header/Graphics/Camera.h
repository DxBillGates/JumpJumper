#pragma once
#include "..\..\Header\Math\Math.h"
#include "..\..\Header\Window\Window.h"
#include "..\..\Header\Input\Input.h"
#include "GraphicsDevice.h"
#include "CBufferStruct.h"
namespace GatesEngine
{
	//基本となるカメラクラス、基本定点カメラにしか使えない
	class Camera
	{
	protected:
		GraphicsDevice* pGraphicsDevice;
		Window* pMainWindow;

		Math::Matrix4x4 projectionMatrix;
		float nearClip;
		float farClip;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		Camera();

		/// <summary>
		/// 仮想デストラクタ
		/// </summary>
		virtual ~Camera() {}

		/// <summary>
		/// 仮想初期化関数
		/// </summary>
		virtual void Initialize() {}

		/// <summary>
		/// 仮想更新関数
		/// </summary>
		virtual void Update() {}
		
		/// <summary>
		/// 仮想GPUセット関数
		/// </summary>
		/// <param name="setDescriptorIndex">セットするレジスタ番号</param>
		virtual void Set(int setDescriptorIndex) {}

		/// <summary>
		/// アプリケーションで使用しているGraphicsDeviceのポインタをセットする関数
		/// </summary>
		/// <param name="gDevice">GraphicsDeviceのポインタ</param>
		void SetGraphicsDevice(GraphicsDevice* gDevice);

		/// <summary>
		/// アプリケーションで使用しているMainWindowのポインタ
		/// </summary>
		/// <param name="window">Windowのポインタ</param>
		void SetMainWindow(Window* window);

		/// <summary>
		/// プロジェクション行列の設定
		/// </summary>
		/// <param name="matrix">プロジェクション行列</param>
		void SetProjection(const Math::Matrix4x4& matrix);

		/// <summary>
		/// ニア―クリップとファークリップの設定
		/// </summary>
		/// <param name="n">ニア―クリップの値</param>
		/// <param name="f">ファークリップの値</param>
		void SetClip(float n = 1, float f = 10000);
	};
}