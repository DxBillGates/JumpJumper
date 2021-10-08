#pragma once
#include <d3d12.h>
#include <dxgi1_6.h>
#include <vector>
#include "..\Math\Vector2.h"
#include "..\Math\Vector4.h"
#include "..\Window\Window.h"

namespace GatesEngine
{
	class DescriptorHeapManager;
	class RenderTarget;
	class ShaderManager;
	class MeshManager;
	class CBVSRVUAVHeap;
	class CBufferAllocater;
	class DepthStencil;
	class GraphicsDevice
	{
	private:
		using Vector2 = GatesEngine::Math::Vector2;
		using Vector4 = GatesEngine::Math::Vector4;

		D3D12_VIEWPORT               mViewport;
		D3D12_RECT                   mRect;
		IDXGIFactory6*               mDxgiFactory;
		std::vector<IDXGIAdapter*>   mEnumAdapter;
		IDXGIAdapter*                mUseAdapter;
		ID3D12Device*                mDevice;
		ID3D12CommandAllocator*      mCmdAlloc;
		ID3D12GraphicsCommandList*   mCmdList;
		ID3D12CommandQueue*          mCmdQueue;
		IDXGISwapChain4*             mSwapChain;
		std::vector<ID3D12Resource*> mFrameBuffer;
		RenderTarget*                mRenderTarget;
		ID3D12DescriptorHeap*        mDsvHeap;
		ID3D12Resource*              mDepthBuffer;
		ID3D12Fence*                 mFence;
		UINT64                       mFenceValue;

		DescriptorHeapManager* descriptorHeapManager;
		ShaderManager* shaderManager;
		MeshManager* meshManager;
		CBVSRVUAVHeap* cbvSrvUavHeap;
		CBufferAllocater* cBufferAllocater;
	private:
		void CreateDxgiFactory();
		void CreateDevice();
		void CreateCmdList();
		void CreateSwapChain(HWND hwnd);
		void CreateRtv();
		void CreateDsv();
		void CreateFence();
	public:
		GraphicsDevice();
		~GraphicsDevice();

		bool Create(Window* mainWindow);
		void ClearRenderTarget(const Vector4& color = { 0,0,0,0 },bool clearFlag = true, RenderTarget* renderTarget = nullptr,DepthStencil* depthStencil = nullptr);
		void ClearRenderTargetOutDsv(const Vector4& color = { 0,0,0,0 }, bool clearFlag = true, RenderTarget* renderTarget = nullptr);
		void ClearDepthStencil();
		bool ScreenFlip();
		void SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after);
		void SetViewport(const Vector2& size, const Vector2& pos = {});
		void SetDescriptorHeap();
		void SetMultiRenderTarget(std::vector<RenderTarget*> renderTargets);

		ID3D12Device* GetDevice();
		ID3D12GraphicsCommandList* GetCmdList();
		IDXGISwapChain4* GetSwapChain();
		ID3D12Resource* GetCurrentFrameBuffer();
		ID3D12Resource* GetDepthBuffer();
		ID3D12DescriptorHeap* GetRtvHeap();
		DescriptorHeapManager* GetDescriptorHeapManager();
		ShaderManager* GetShaderManager();
		MeshManager* GetMeshManager();
		CBVSRVUAVHeap* GetCBVSRVUAVHeap();
		CBufferAllocater* GetCBufferAllocater();
	};
}
