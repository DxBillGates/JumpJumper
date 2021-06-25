#include "..\..\Header\Graphics\RenderTarget.h"
#include "..\..\Header\Graphics\GraphicsDevice.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::RenderTarget::RenderTarget()
	:pGraphicsDevice(nullptr)
	,rtvHeap(nullptr)
	,currentResourceState(D3D12_RESOURCE_STATES())
{
}

GatesEngine::RenderTarget::~RenderTarget()
{
	COM_RELEASE(rtvHeap);
}

ID3D12DescriptorHeap* GatesEngine::RenderTarget::GetHeap()
{
	return rtvHeap;
}

bool GatesEngine::RenderTarget::Create(GraphicsDevice* wrapper, std::vector<ID3D12Resource*>* resources)
{
	pResources.resize((int)(*resources).size());
	//ヒープの生成
	HRESULT result;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = (int)(*resources).size();
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	result = wrapper->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	//ビューの生成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	int i = 0;
	for (auto& r : *resources)
	{
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHeapHandle.ptr += (UINT64)i * wrapper->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
		wrapper->GetDevice()->CreateRenderTargetView((*resources)[i], &rtvDesc, rtvHeapHandle);
		pResources[i] = (*resources)[i];
		++i;
	}
	pGraphicsDevice = wrapper;
	return false;
}

bool GatesEngine::RenderTarget::Create(GraphicsDevice* wrapper, ID3D12Resource* resource)
{
	pResources.resize(1);
	//ヒープの生成
	HRESULT result;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	result = wrapper->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	//ビューの生成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	wrapper->GetDevice()->CreateRenderTargetView(resource, &rtvDesc, rtvHeapHandle);
	pResources[0] = resource;
	pGraphicsDevice = wrapper;
	return false;
}

D3D12_RESOURCE_STATES GatesEngine::RenderTarget::GetCurrentResourceState()
{
	return currentResourceState;
}

void GatesEngine::RenderTarget::SetCurrentResourceState(D3D12_RESOURCE_STATES state)
{
	currentResourceState = state;
}

ID3D12Resource* GatesEngine::RenderTarget::GetResource()
{
	return pResources[0];
}
