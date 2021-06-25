#include "..\..\Header\Graphics\DepthStencil.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::DepthStencil::DepthStencil()
	: graphicsDevice(nullptr)
	, depthBuffer(nullptr)
	, heap(nullptr)
{
}

GatesEngine::DepthStencil::~DepthStencil()
{
	COM_RELEASE(depthBuffer);
	COM_RELEASE(heap);
}

void GatesEngine::DepthStencil::CreateBuffer(GraphicsDevice* pGraphicsDevice, const Math::Vector2& size)
{
	graphicsDevice = pGraphicsDevice;
	//デプスバッファの設定
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resDesc.Width = (UINT64)size.x;
	resDesc.Height = (UINT64)size.y;
	resDesc.DepthOrArraySize = 1;
	resDesc.Format = DXGI_FORMAT_R32_TYPELESS;
	resDesc.SampleDesc.Count = 1;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;
	//デプスバッファのフォーマットと最大深度値を設定
	D3D12_CLEAR_VALUE depthClearValue = {};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	//設定をもとにデプスバッファを生成
	result = graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_DEPTH_WRITE, &depthClearValue, IID_PPV_ARGS(&depthBuffer));
	//DSVを格納するためのヒープの設定
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	//DSVヒープの生成
	result = graphicsDevice->GetDevice()->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&heap));
	//DSVの生成
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	graphicsDevice->GetDevice()->CreateDepthStencilView(depthBuffer, &dsvDesc, heap->GetCPUDescriptorHandleForHeapStart());
}

ID3D12Resource* GatesEngine::DepthStencil::GetResource()
{
	return depthBuffer;
}

ID3D12DescriptorHeap* GatesEngine::DepthStencil::GetHeap()
{
	return heap;
}

void GatesEngine::DepthStencil::Clear()
{
	graphicsDevice->GetCmdList()->ClearDepthStencilView(heap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);
}
