#include "..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::CBVSRVUAVHeap::CBVSRVUAVHeap()
	: pDevice(nullptr)
	, pCmdList(nullptr)
	, heap(nullptr)
	, useCount({})
	, isCreated(false)
	, incrementSize(0)
	, nextSrvDescriptorNum(0)
{
}

GatesEngine::CBVSRVUAVHeap::~CBVSRVUAVHeap()
{
	COM_RELEASE(heap);
}

void GatesEngine::CBVSRVUAVHeap::SetGraphicsDevice(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList)
{
	pDevice = device;
	pCmdList = cmdList;
}

void GatesEngine::CBVSRVUAVHeap::Create(const Math::Vector3& useCount)
{
	if (isCreated)return;
	if (!pDevice)return;

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NodeMask = 0;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = (int)useCount.x + (int)useCount.y + (int)useCount.z;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	HRESULT result = pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

	this->useCount = useCount;
	incrementSize = pDevice->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
}

ID3D12DescriptorHeap* GatesEngine::CBVSRVUAVHeap::GetHeap()
{
	return heap;
}

GatesEngine::Math::Vector3 GatesEngine::CBVSRVUAVHeap::GetUseCount()
{
	return useCount;
}

void GatesEngine::CBVSRVUAVHeap::CreateSRV(ID3D12Resource* buffer)
{
	if (!pDevice)return;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += ((UINT64)useCount.x + 1) * incrementSize + (UINT64)nextSrvDescriptorNum * incrementSize;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = buffer->GetDesc().Format;
	if (buffer->GetDesc().Format == DXGI_FORMAT_R32_TYPELESS)
	{
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pDevice->CreateShaderResourceView(buffer, &srvDesc, handle);
	++nextSrvDescriptorNum;
}

void GatesEngine::CBVSRVUAVHeap::CreateSRV(ID3D12Resource* buffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc)
{
	if (!pDevice)return;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += ((UINT64)useCount.x + 1) * incrementSize + (UINT64)nextSrvDescriptorNum * incrementSize;
	pDevice->CreateShaderResourceView(buffer, &viewDesc, handle);
	++nextSrvDescriptorNum;
}

D3D12_GPU_DESCRIPTOR_HANDLE GatesEngine::CBVSRVUAVHeap::GetSRVHandleForSRV(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize * ((UINT64)useCount.x + 1);
	handle.ptr += (UINT64)incrementSize * number;
	return handle;
}

int GatesEngine::CBVSRVUAVHeap::GetNextSrvNumber()
{
	return nextSrvDescriptorNum;
}

void GatesEngine::CBVSRVUAVHeap::Set()
{
	ID3D12DescriptorHeap* ppHeaps[] = { heap };
	pCmdList->SetDescriptorHeaps(1, ppHeaps);
}
