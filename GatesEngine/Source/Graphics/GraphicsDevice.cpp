#include "..\..\Header\Graphics\GraphicsDevice.h"
#include "..\..\Header\Graphics\COMRelease.h"
#include "..\..\Header\Graphics\RenderTarget.h"
#include "..\..\Header\Graphics\DescriptorHeapManager.h"
#include "..\..\Header\Graphics\Manager\MeshManager.h"
#include "..\..\Header\Graphics\Manager\ShaderManager.h"
#include "..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\Header\Graphics\CBufferAllocater.h"
#include "..\..\Header\Graphics\DepthStencil.h"

#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

#include <string>

GatesEngine::GraphicsDevice::GraphicsDevice()
	: mViewport({})
	, mRect({})
	, mDxgiFactory(nullptr)
	, mEnumAdapter(std::vector<IDXGIAdapter*>())
	, mUseAdapter(nullptr)
	, mDevice(nullptr)
	, mCmdAlloc(nullptr)
	, mCmdList(nullptr)
	, mCmdQueue(nullptr)
	, mSwapChain(nullptr)
	, mFrameBuffer(std::vector<ID3D12Resource*>())
	, mRenderTarget(nullptr)
	, mDsvHeap(nullptr)
	, mDepthBuffer(nullptr)
	, mFence(nullptr)
	, mFenceValue(0)
	, descriptorHeapManager(nullptr)
	, shaderManager(nullptr)
	, meshManager(nullptr)
	, cbvSrvUavHeap(nullptr)
	, cBufferAllocater(nullptr)
{
#ifdef _DEBUG
	HRESULT result;
	ID3D12Debug* debug;
	if (SUCCEEDED(result = D3D12GetDebugInterface(IID_PPV_ARGS(&debug))))
	{
		debug->EnableDebugLayer();
		COM_RELEASE(debug);
	}
#endif
}

GatesEngine::GraphicsDevice::~GraphicsDevice()
{
	//���s���̃R�}���h���I�����Ă�����!!
	mCmdQueue->Signal(mFence, ++mFenceValue);
	if (mFence->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	COM_RELEASE(mDxgiFactory);
	COM_RELEASE(mDevice);
	COM_RELEASE(mCmdAlloc);
	COM_RELEASE(mCmdList);
	COM_RELEASE(mCmdQueue);
	COM_RELEASE(mFrameBuffer[0]);
	COM_RELEASE(mFrameBuffer[1]);
	COM_RELEASE(mSwapChain);
	delete mRenderTarget;
	COM_RELEASE(mDepthBuffer);
	COM_RELEASE(mDsvHeap);
	COM_RELEASE(mFence);

	delete descriptorHeapManager;
	delete shaderManager;
	delete meshManager;
	delete cBufferAllocater;
	delete cbvSrvUavHeap;
}

bool GatesEngine::GraphicsDevice::Create(Window* mainWindow)
{
	SetViewport(mainWindow->GetWindowSize(), Vector2());
	CreateDxgiFactory();
	CreateDevice();
	CreateCmdList();
	CreateSwapChain(mainWindow->GetHandle());
	CreateRtv();
	CreateDsv();
	CreateFence();

	descriptorHeapManager = new DescriptorHeapManager(this, 640, 64);
	shaderManager = new ShaderManager(this);
	meshManager = new MeshManager(this);
	cbvSrvUavHeap = new CBVSRVUAVHeap();
	cBufferAllocater = new CBufferAllocater();

	cbvSrvUavHeap->SetGraphicsDevice(this);
	cbvSrvUavHeap->Create({ 100000,256,0 });

	cBufferAllocater->SetGraphicsDevice(this);
	cBufferAllocater->SetHeap(cbvSrvUavHeap);
	cBufferAllocater->Create();


	return true;
}

void GatesEngine::GraphicsDevice::ClearRenderTarget(const Vector4& color, bool clearFlag, RenderTarget* renderTarget, DepthStencil* depthStencil)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//useRenderTarget��nullptr�̏ꍇ�̓o�b�N�o�b�t�@�������_�\�^�[�Q�b�g�Ƃ��邩�[�x��������������
	if (renderTarget == nullptr)
	{
		renderTarget = mRenderTarget;
		if (renderTarget->GetCurrentResourceState() != D3D12_RESOURCE_STATE_RENDER_TARGET)
		{
			renderTarget->SetCurrentResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET);
			SetResourceBarrier(mFrameBuffer[mSwapChain->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}
		rtvHandle = renderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += (UINT64)mSwapChain->GetCurrentBackBufferIndex() * mDevice->GetDescriptorHandleIncrementSize(renderTarget->GetHeap()->GetDesc().Type);
		if (depthStencil)
		{
			dsvHandle = depthStencil->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
		}

		mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	}
	else
	{
		renderTarget->Prepare();
		rtvHandle = renderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		if (depthStencil)
		{
			dsvHandle = depthStencil->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		}
		else
		{
			dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
		}
		mCmdList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);
	}

	if (clearFlag)
	{
		//useRenderTarget��color�œh��Ԃ�
		Vector4 setColor = color;
		if (setColor.x > 1)setColor.x /= 255.0f;
		if (setColor.y > 1)setColor.y /= 255.0f;
		if (setColor.z > 1)setColor.z /= 255.0f;

		float rgba[] = { setColor.x,setColor.y,setColor.z,setColor.w };
		mCmdList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);

		if (depthStencil)
		{
			depthStencil->Clear();
		}
		else
		{
			ClearDepthStencil();
		}

	}

	D3D12_VIEWPORT viewport = mViewport;
	D3D12_RECT rect = mRect;
	D3D12_RESOURCE_DESC resDesc = renderTarget->GetResource()->GetDesc();

	viewport.Height = (float)resDesc.Height;
	viewport.Width = (float)resDesc.Width;

	rect.bottom = (int)resDesc.Height;
	rect.right = (int)resDesc.Width;

	mCmdList->RSSetViewports(1, &viewport);
	mCmdList->RSSetScissorRects(1, &rect);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GatesEngine::GraphicsDevice::ClearRenderTargetOutDsv(const Vector4& color, bool clearFlag, RenderTarget* renderTarget)
{
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle;
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;

	//useRenderTarget��nullptr�̏ꍇ�̓o�b�N�o�b�t�@�������_�\�^�[�Q�b�g�Ƃ���
	if (renderTarget == nullptr)
	{
		renderTarget = mRenderTarget;
		if (renderTarget->GetCurrentResourceState() != D3D12_RESOURCE_STATE_RENDER_TARGET)
		{
			renderTarget->SetCurrentResourceState(D3D12_RESOURCE_STATE_RENDER_TARGET);
			SetResourceBarrier(mFrameBuffer[mSwapChain->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET);
		}
		rtvHandle = renderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += (UINT64)mSwapChain->GetCurrentBackBufferIndex() * mDevice->GetDescriptorHandleIncrementSize(renderTarget->GetHeap()->GetDesc().Type);
		dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
		mCmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
		//ClearDepthStencil();
	}
	else
	{
		renderTarget->Prepare();
		rtvHandle = renderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
		mCmdList->OMSetRenderTargets(1, &rtvHandle, false, nullptr);
	}

	if (clearFlag)
	{
		//useRenderTarget��color�œh��Ԃ�
		Vector4 setColor = color;
		if (setColor.x > 1)setColor.x /= 255.0f;
		if (setColor.y > 1)setColor.y /= 255.0f;
		if (setColor.z > 1)setColor.z /= 255.0f;

		float rgba[] = { setColor.x,setColor.y,setColor.z,setColor.w };
		mCmdList->ClearRenderTargetView(rtvHandle, rgba, 0, nullptr);
	}


	D3D12_VIEWPORT viewport = mViewport;
	D3D12_RECT rect = mRect;
	D3D12_RESOURCE_DESC resDesc = renderTarget->GetResource()->GetDesc();

	viewport.Height = (float)resDesc.Height;
	viewport.Width = (float)resDesc.Width;

	rect.bottom = (int)resDesc.Height;
	rect.right = (int)resDesc.Width;

	mCmdList->RSSetViewports(1, &viewport);
	mCmdList->RSSetScissorRects(1, &rect);
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void GatesEngine::GraphicsDevice::ClearDepthStencil()
{
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = mDsvHeap->GetCPUDescriptorHandleForHeapStart();
	mCmdList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}

void GatesEngine::GraphicsDevice::ScreenFlip()
{
	if (mRenderTarget->GetCurrentResourceState() != D3D12_RESOURCE_STATE_PRESENT)
	{
		mRenderTarget->SetCurrentResourceState(D3D12_RESOURCE_STATE_PRESENT);
		SetResourceBarrier(mFrameBuffer[mSwapChain->GetCurrentBackBufferIndex()], D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT);
	}
	mCmdList->Close();
	ID3D12CommandList* cmdLists[] = { mCmdList };
	mCmdQueue->ExecuteCommandLists(1, cmdLists);
	mCmdQueue->Signal(mFence, ++mFenceValue);
	if (mFence->GetCompletedValue() != mFenceValue)
	{
		HANDLE event = CreateEvent(nullptr, false, false, nullptr);
		mFence->SetEventOnCompletion(mFenceValue, event);
		WaitForSingleObject(event, INFINITE);
		CloseHandle(event);
	}
	mCmdAlloc->Reset();
	mCmdList->Reset(mCmdAlloc, nullptr);
	mSwapChain->Present(0, 0);
}

void GatesEngine::GraphicsDevice::SetResourceBarrier(ID3D12Resource* resource, D3D12_RESOURCE_STATES before, D3D12_RESOURCE_STATES after)
{
	D3D12_RESOURCE_BARRIER barrier = {};
	barrier.Transition.pResource = resource;
	barrier.Transition.StateAfter = after;
	barrier.Transition.StateBefore = before;
	mCmdList->ResourceBarrier(1, &barrier);
}

void GatesEngine::GraphicsDevice::SetViewport(const Vector2& size, const Vector2& pos)
{
	mViewport = { 0,0,(float)size.x,(float)size.y,0,1.0f };
	mRect = { 0,0,(int)size.x,(int)size.y };
}

void GatesEngine::GraphicsDevice::SetDescriptorHeap()
{
	descriptorHeapManager->Set();
}

void GatesEngine::GraphicsDevice::SetMultiRenderTarget(std::vector<RenderTarget*> renderTargets)
{
	int renderTargetSize = (int)renderTargets.size();
	std::vector<D3D12_CPU_DESCRIPTOR_HANDLE> handles;
	handles.resize(renderTargetSize);

	std::vector<D3D12_VIEWPORT> viewports;
	std::vector<D3D12_RECT> rects;

	viewports.resize(renderTargetSize);
	rects.resize(renderTargetSize);

	for (int i = 0; i < renderTargetSize; ++i)
	{
		renderTargets[i]->Prepare();
		handles[i] = renderTargets[i]->GetHeap()->GetCPUDescriptorHandleForHeapStart();

		D3D12_RESOURCE_DESC resDesc = renderTargets[i]->GetResource()->GetDesc();

		viewports[i] = mViewport;
		viewports[i].Height = (float)resDesc.Height;
		viewports[i].Width = (float)resDesc.Width;

		rects[i] = mRect;
		rects[i].bottom = (int)resDesc.Height;
		rects[i].right = (int)resDesc.Width;

		//useRenderTarget��color�œh��Ԃ�
		float rgba[] = { 1,1,1,1 };
		mCmdList->ClearRenderTargetView(handles[i], rgba, 0, nullptr);
	}


	mCmdList->OMSetRenderTargets(renderTargetSize, handles.data(), false, nullptr);

	mCmdList->RSSetViewports(renderTargetSize, viewports.data());
	mCmdList->RSSetScissorRects(renderTargetSize, rects.data());
	mCmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

ID3D12Device* GatesEngine::GraphicsDevice::GetDevice()
{
	return mDevice;
}

ID3D12GraphicsCommandList* GatesEngine::GraphicsDevice::GetCmdList()
{
	return mCmdList;
}

IDXGISwapChain4* GatesEngine::GraphicsDevice::GetSwapChain()
{
	return mSwapChain;
}

ID3D12Resource* GatesEngine::GraphicsDevice::GetCurrentFrameBuffer()
{
	return mFrameBuffer[mSwapChain->GetCurrentBackBufferIndex()];
}

ID3D12Resource* GatesEngine::GraphicsDevice::GetDepthBuffer()
{
	return mDepthBuffer;
}

ID3D12DescriptorHeap* GatesEngine::GraphicsDevice::GetRtvHeap()
{
	return nullptr;
}

GatesEngine::DescriptorHeapManager* GatesEngine::GraphicsDevice::GetDescriptorHeapManager()
{
	return descriptorHeapManager;
}

GatesEngine::ShaderManager* GatesEngine::GraphicsDevice::GetShaderManager()
{
	return shaderManager;
}

GatesEngine::MeshManager* GatesEngine::GraphicsDevice::GetMeshManager()
{
	return meshManager;
}

GatesEngine::CBVSRVUAVHeap* GatesEngine::GraphicsDevice::GetCBVSRVUAVHeap()
{
	return cbvSrvUavHeap;
}

GatesEngine::CBufferAllocater* GatesEngine::GraphicsDevice::GetCBufferAllocater()
{
	return cBufferAllocater;
}

void GatesEngine::GraphicsDevice::CreateDxgiFactory()
{
	//DXGIFactory�̐���
	HRESULT result;
	result = CreateDXGIFactory(IID_PPV_ARGS(&mDxgiFactory));
}

void GatesEngine::GraphicsDevice::CreateDevice()
{
	//�g�p�ł���GPU��T���Ă��̃A�_�v�^��ID3DDevice�𐶐�
	HRESULT result;
	for (int i = 0; mDxgiFactory->EnumAdapters(i, &mUseAdapter) != DXGI_ERROR_NOT_FOUND; ++i)
	{
		mEnumAdapter.push_back(mUseAdapter);
	}
	DXGI_ADAPTER_DESC adapterDesc = {};
	for (int i = 0; i < (int)mEnumAdapter.size(); ++i)
	{
		mEnumAdapter[i]->GetDesc(&adapterDesc);
		std::wstring adapterName = adapterDesc.Description;
		if (adapterName.find(L"Microsoft") == std::wstring::npos && adapterName.find(L"Intel") == std::wstring::npos)
		{
			mUseAdapter = mEnumAdapter[i];
			break;
		}
	}
	D3D_FEATURE_LEVEL featureLevel[] = { D3D_FEATURE_LEVEL_12_1, D3D_FEATURE_LEVEL_12_0, D3D_FEATURE_LEVEL_11_1, D3D_FEATURE_LEVEL_11_0 };
	for (int i = 0; i < _countof(featureLevel); ++i)
	{
		if (SUCCEEDED(result = D3D12CreateDevice(mUseAdapter, featureLevel[i], IID_PPV_ARGS(&mDevice))))break;
	}
}

void GatesEngine::GraphicsDevice::CreateCmdList()
{
	//�R�}���h�֌W�𐶐�
	HRESULT result;
	result = mDevice->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&mCmdAlloc));
	result = mDevice->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, mCmdAlloc, nullptr, IID_PPV_ARGS(&mCmdList));
	D3D12_COMMAND_QUEUE_DESC cmdQueueDesc = {};
	result = mDevice->CreateCommandQueue(&cmdQueueDesc, IID_PPV_ARGS(&mCmdQueue));
}

void GatesEngine::GraphicsDevice::CreateSwapChain(HWND hwnd)
{
	//Application����gameWindow�̃n���h�����擾�����̃n���h���Ƃ��̂ق��̐ݒ肩��SwapChain�𐶐�
	HRESULT result;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.Width = (UINT)mViewport.Width;
	swapChainDesc.Height = (UINT)mViewport.Height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	result = mDxgiFactory->CreateSwapChainForHwnd(mCmdQueue, hwnd, &swapChainDesc, nullptr, nullptr, (IDXGISwapChain1**)&mSwapChain);
}

void GatesEngine::GraphicsDevice::CreateRtv()
{
	//mRenderTarget = new MyDirectX::Dx12RenderTarget(this, 2);
	HRESULT result;
	//mFrameBuffer.resize(2);
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	//rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	////SwapChain����o�b�N�o�b�t�@�A�t�����g�o�b�t�@���擾����������Ƃ�RTV�𐶐�
	//for (int i = 0; i < (int)mFrameBuffer.size(); ++i)
	//{
	//	result = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mFrameBuffer[i]));
	//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = mRenderTarget->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	//	rtvHeapHandle.ptr += i * mDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//	mDevice->CreateRenderTargetView(mFrameBuffer[i], &rtvDesc, rtvHeapHandle);
	//}

	mRenderTarget = new RenderTarget();
	mFrameBuffer.resize(2);
	for (int i = 0; i < (int)mFrameBuffer.size(); ++i)
	{
		result = mSwapChain->GetBuffer(i, IID_PPV_ARGS(&mFrameBuffer[i]));
	}

	mRenderTarget->Create(this, &mFrameBuffer);
}

void GatesEngine::GraphicsDevice::CreateDsv()
{
	//�f�v�X�o�b�t�@�̐ݒ�
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = (UINT64)mViewport.Width;
	resDesc.Height = (UINT64)mViewport.Height;
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//�f�v�X�o�b�t�@�̃t�H�[�}�b�g�ƍő�[�x�l��ݒ�
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//�ݒ�����ƂɃf�v�X�o�b�t�@�𐶐�
	result = mDevice->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&mDepthBuffer));
	//DSV���i�[���邽�߂̃q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DSV�q�[�v�̐���
	result = mDevice->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&mDsvHeap));
	//DSV�̐���
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	mDevice->CreateDepthStencilView(mDepthBuffer, &dsvDesc, mDsvHeap->GetCPUDescriptorHandleForHeapStart());
}

void GatesEngine::GraphicsDevice::CreateFence()
{
	//CPU��GPU�̓�������������������t�F���X�𐶐�
	HRESULT result;
	result = mDevice->CreateFence(mFenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&mFence));
}
