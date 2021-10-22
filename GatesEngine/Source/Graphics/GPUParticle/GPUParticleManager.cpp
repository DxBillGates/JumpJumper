#include "..\..\..\Header\Graphics\GPUParticle\GPUParticleManager.h"
#include "..\..\..\Header\Graphics\COMRelease.h"

GatesEngine::GPUParticleManager::GPUParticleManager(GraphicsDevice* pGraphicsDevice)
	: maxParticleValue(1000000)
	, nextParticleOffset(0)
	, maxDescriptorValue(400)
	, uavNextOffset(0)
	, srvNextOffset(0)
	, particleBuffer(nullptr)
	, updateParticleBuffer(nullptr)
	, heap(nullptr)
	, graphicsDevice(pGraphicsDevice)
{
	//生成処理
	HRESULT hr;
	//リソースの生成
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProp.CreationNodeMask = 1;
	heapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
	heapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProp.VisibleNodeMask = 0;

	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Alignment = 0;
	resDesc.DepthOrArraySize = 1;
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_UNORDERED_ACCESS;
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Height = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc = { 1,0 };
	resDesc.Width = (sizeof(ParticleData) * maxParticleValue + 0xff) & ~0xff;

	hr = graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_UNORDERED_ACCESS, nullptr, IID_PPV_ARGS(&particleBuffer));

	//SRV用バッファ生成
	resDesc.Flags = {};
	resDesc.Format = DXGI_FORMAT_UNKNOWN;
	resDesc.Width = sizeof(ParticleData) * maxParticleValue;

	hr = graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&updateParticleBuffer));

	//ヒープの生成
	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	heapDesc.NodeMask = 0;
	heapDesc.NumDescriptors = maxDescriptorValue;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;

	hr = graphicsDevice->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));
}

GatesEngine::GPUParticleManager::~GPUParticleManager()
{
	COM_RELEASE(particleBuffer);
	COM_RELEASE(updateParticleBuffer);
	COM_RELEASE(heap);
}

UINT GatesEngine::GPUParticleManager::GetMaxParticleValue()
{
	return maxParticleValue;
}

UINT GatesEngine::GPUParticleManager::GetNextParticleOffset()
{
	return nextParticleOffset;
}

UINT GatesEngine::GPUParticleManager::GetUavNextOffset()
{
	return uavNextOffset;
}

UINT GatesEngine::GPUParticleManager::GetSrvNextOffset()
{
	return srvNextOffset;
}

ID3D12Resource* GatesEngine::GPUParticleManager::GetParticleBuffer()
{
	return particleBuffer;
}

ID3D12Resource* GatesEngine::GPUParticleManager::GetUpdateParticleBuffer()
{
	return updateParticleBuffer;
}

ID3D12DescriptorHeap* GatesEngine::GPUParticleManager::GetHeap()
{
	return heap;
}

void GatesEngine::GPUParticleManager::IncrementParticleOffset(UINT value)
{
	nextParticleOffset += value;
}

void GatesEngine::GPUParticleManager::CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc)
{
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * uavNextOffset;
	graphicsDevice->GetDevice()->CreateUnorderedAccessView(particleBuffer, nullptr, &viewDesc, handle);

	++uavNextOffset;
}

void GatesEngine::GPUParticleManager::CreateSRV(ID3D12Resource* buffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc)
{
	auto handle = heap->GetCPUDescriptorHandleForHeapStart();
	UINT incrementSize = graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	handle.ptr += (UINT64)incrementSize * 200 + (UINT64)incrementSize * srvNextOffset;
	graphicsDevice->GetDevice()->CreateShaderResourceView(buffer, &viewDesc, handle);

	++srvNextOffset;
}

GatesEngine::GraphicsDevice* GatesEngine::GPUParticleManager::GetDevice()
{
	return graphicsDevice;
}