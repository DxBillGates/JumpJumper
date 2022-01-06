#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include "..\Math\Vector2.h"
#include "..\Math\Vector4.h"
#include "..\Window\Window.h"

namespace GatesEngine
{
	class RenderTarget;
	class CBVSRVUAVHeap;
	class CBufferAllocater;
	class DepthStencil;
	class Camera;
	class GPUParticleManager;
	class GraphicsDevice
	{
	private:
		using Vector2 = GatesEngine::Math::Vector2;
		using Vector4 = GatesEngine::Math::Vector4;

		D3D12_VIEWPORT               mViewport;
		D3D12_RECT                   mRect;
		IDXGIFactory6* mDxgiFactory;
		std::vector<IDXGIAdapter*>   mEnumAdapter;
		IDXGIAdapter* mUseAdapter;
		ID3D12Device* mDevice;
		ID3D12CommandAllocator* mCmdAlloc;
		ID3D12GraphicsCommandList* mCmdList;
		ID3D12CommandQueue* mCmdQueue;
		IDXGISwapChain4* mSwapChain;
		std::vector<ID3D12Resource*> mFrameBuffer;
		RenderTarget* mRenderTarget;
		ID3D12DescriptorHeap* mDsvHeap;
		ID3D12Resource* mDepthBuffer;
		ID3D12Fence* mFence;
		UINT64                       mFenceValue;

		CBVSRVUAVHeap* cbvSrvUavHeap;
		CBufferAllocater* cBufferAllocater;

		Camera* mainCamera;
		GPUParticleManager* particleManager;
	private:
		void CreateDxgiFactory();
		void CreateDevice();
		void CreateCmdList();
		void CreateSwapChain(HWND hwnd);
		void CreateRtv();
		void CreateDsv();
		void CreateFence();
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		GraphicsDevice();
		~GraphicsDevice();

		/// <summary>
		/// 各種デバイス生成関数
		/// </summary>
		/// <param name="mainWindow">メインウィンドウのポインタ</param>
		/// <returns></returns>
		bool Create(Window* mainWindow);

		/// <summary>
		/// レンダ―ターゲットと深度バッファの情報をクリアする命令をコマンドリストにセットする関数
		/// </summary>
		/// <param name="color">指定色で画面を塗りつぶす</param>
		/// <param name="clearFlag">レンダ―ターゲットと深度バッファをクリアするかのフラグ</param>
		/// <param name="renderTarget">RenderTargetクラスのポインタ、nullptrの場合このインスタンスが保持しているバックバッファがセットされる</param>
		/// <param name="depthStencil">DepthStencilクラスのポインタ、nullptrの場合このインスタンスが保持している深度バッファがセットされる</param>
		void ClearRenderTarget(const Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* renderTarget = nullptr, DepthStencil* depthStencil = nullptr);

		/// <summary>
		/// レンダ―ターゲットの情報をクリアする命令をコマンドリストにセットする関数
		/// </summary>
		/// <param name="color">指定色で塗りつぶす</param>
		/// <param name="clearFlag">クリアするかのフラグ</param>
		/// <param name="renderTarget">RenderTargetクラスのポインタ、nullptrの場合このインスタンスが保持しているバックバッファがセットされる</param>
		void ClearRenderTargetOutDsv(const Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* renderTarget = nullptr);

		/// <summary>
		/// このインスタンスが保持している深度バッファをクリアする
		/// </summary>
		void ClearDepthStencil();

		/// <summary>
		/// コマンドリストに積まれているコマンドを順に実行していく、またその命令が終わるまでアプリケーションを待機＆垂直同期
		/// </summary>
		/// <returns>成否</returns>
		bool ScreenFlip();

		/// <summary>
		/// resourceとして引数に渡したバッファの扱いをを安全に変更する関数
		/// </summary>
		/// <param name="resource">指定バッファ</param>
		/// <param name="before">現在の状態</param>
		/// <param name="after">新しい状態</param>
		void SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);

		/// <summary>
		/// ビューポートをセットする関数
		/// </summary>
		/// <param name="size">サイズ</param>
		/// <param name="pos">初期位置</param>
		void SetViewport(const Vector2& size, const Vector2& pos = {});

		/// <summary>
		/// レンダ―ターゲットを複数セットする関数
		/// </summary>
		/// <param name="renderTargets">セットするレンダ―ターゲットの配列  例) {renderTarget1,renderTarget2}</param>
		/// <param name="depthStencil">セットする深度バッファ、nullptrの場合このインスタンスが保持している深度がセットされる</param>
		/// <param name="clearColor">指定色塗りつぶす</param>
		void SetMultiRenderTarget(std::vector<RenderTarget*> renderTargets, DepthStencil* depthStencil = nullptr, const Math::Vector4& clearColor = {0,0,0,1});

		/// <summary>
		/// メインカメラのポインタをセットする関数
		/// </summary>
		/// <param name="camera"></param>
		void SetMainCamera(Camera* camera);

		/// <summary>
		/// ID3D12Deviceのポインタを返す関数
		/// </summary>
		/// <returns>ID3D12Deviceのポインタ</returns>
		ID3D12Device* GetDevice();

		/// <summary>
		/// ID3D12GraphicsCommandlistのポインタを返す関数
		/// </summary>
		/// <returns>ID3D12GraphicsCommandlistのポインタ</returns>
		ID3D12GraphicsCommandList* GetCmdList();

		/// <summary>
		/// IDXGISwapChain4のポインタを返す関数
		/// </summary>
		/// <returns>IDXGISwapChain4を返す関数</returns>
		IDXGISwapChain4* GetSwapChain();

		/// <summary>
		/// フレームバッファのポインタを返す関数
		/// </summary>
		/// <returns>フレームバッファのポインタ</returns>
		ID3D12Resource* GetCurrentFrameBuffer();

		/// <summary>
		/// 深度バッファのポインタを返す関数
		/// </summary>
		/// <returns>深度バッファのポインタ</returns>
		ID3D12Resource* GetDepthBuffer();

		/// <summary>
		/// RenderTarget用のDescriptorHeapのポインタを返す関数
		/// </summary>
		/// <returns>RenderTarget用のDescriptorHeapのポインタ</returns>
		ID3D12DescriptorHeap* GetRtvHeap();

		/// <summary>
		/// CBVSRVUAVクラスのポインタを返す関数
		/// </summary>
		/// <returns>CBVSRVUAVクラスのポインタ</returns>
		CBVSRVUAVHeap* GetCBVSRVUAVHeap();

		/// <summary>
		/// 定数バッファアロケーターのポインタを返す関数
		/// </summary>
		/// <returns>定数バッファアロケーターのポインタ</returns>
		CBufferAllocater* GetCBufferAllocater();

		/// <summary>
		/// メインカメラのポインタを返す関数
		/// </summary>
		/// <returns>メインカメラのポインタ</returns>
		Camera* GetMainCamera();

		GPUParticleManager* GetParticleManager();
	};
}
