#include "..\..\Header\Graphics\DescriptorHeapManager.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::DescriptorHeapManager::DescriptorHeapManager(GraphicsDevice* graphicsDevice, int cbvNum, int srvNum, int uavNum)
	:pGraphicsDevice(graphicsDevice)
	,mHeap(nullptr)
	,srvStartOffset(cbvNum)
	,incrementSize(pGraphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV))
	,nextCbvDescriptorNum(0)
	,nextSrvDescriptorNum(0)
{
	ID3D12Device* pDevice = graphicsDevice->GetDevice();;
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NodeMask = 0;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	heapDesc.NumDescriptors = cbvNum + srvNum + uavNum;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	pDevice->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&mHeap));
}

GatesEngine::DescriptorHeapManager::~DescriptorHeapManager()
{
	COM_RELEASE(mHeap);
}

void GatesEngine::DescriptorHeapManager::CreateCBV(ID3D12Resource* cBuff)
{
	D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
	cbvDesc.BufferLocation = cBuff->GetGPUVirtualAddress();
	cbvDesc.SizeInBytes = (UINT)cBuff->GetDesc().Width;
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)nextCbvDescriptorNum * incrementSize;
	pGraphicsDevice->GetDevice()->CreateConstantBufferView(&cbvDesc, handle);
	++nextCbvDescriptorNum;
}

void GatesEngine::DescriptorHeapManager::CreateSRV(ID3D12Resource* texBuff)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)srvStartOffset * incrementSize + (UINT64)nextSrvDescriptorNum * incrementSize;
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};

	srvDesc.Format = texBuff->GetDesc().Format;
	if (texBuff->GetDesc().Format == DXGI_FORMAT_R32_TYPELESS)
	{
		srvDesc.Format = DXGI_FORMAT_R32_FLOAT;
	}
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	pGraphicsDevice->GetDevice()->CreateShaderResourceView(texBuff, &srvDesc, handle);
	++nextSrvDescriptorNum;
}

void GatesEngine::DescriptorHeapManager::Set()
{
	pGraphicsDevice->GetCmdList()->SetDescriptorHeaps(1, &mHeap);
}

ID3D12DescriptorHeap* GatesEngine::DescriptorHeapManager::Get()
{
	return mHeap;
}

D3D12_CPU_DESCRIPTOR_HANDLE GatesEngine::DescriptorHeapManager::GetCBVHandleForCPU(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)number * incrementSize;
	return handle;
}

D3D12_GPU_DESCRIPTOR_HANDLE GatesEngine::DescriptorHeapManager::GetCBVHandleForGPU(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = mHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)number * incrementSize;
	return handle;
}

D3D12_CPU_DESCRIPTOR_HANDLE GatesEngine::DescriptorHeapManager::GetSRVHandleForCPU(int number)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = mHeap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize * srvStartOffset;
	handle.ptr += (UINT64)incrementSize * number;
	return handle;;
}

D3D12_GPU_DESCRIPTOR_HANDLE GatesEngine::DescriptorHeapManager::GetSRVHandleForGPU(int number)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handle = mHeap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)incrementSize * srvStartOffset;
	handle.ptr += (UINT64)incrementSize * number;
	return handle;
}

int GatesEngine::DescriptorHeapManager::GetNextCbvDescriptorNumber()
{
	return nextCbvDescriptorNum;
}

int GatesEngine::DescriptorHeapManager::GetNextSrvDescriptorNumber()
{
	return nextSrvDescriptorNum;
}