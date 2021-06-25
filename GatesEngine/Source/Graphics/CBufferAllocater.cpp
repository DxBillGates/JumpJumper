#include "..\..\Header\Graphics\CBufferAllocater.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::CBufferAllocater::~CBufferAllocater()
{
	buffer->Unmap(0, {});
	COM_RELEASE(buffer);
}

void GatesEngine::CBufferAllocater::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}

void GatesEngine::CBufferAllocater::SetHeap(CBVSRVUAVHeap* pHeap)
{
	heap = pHeap;
}

void GatesEngine::CBufferAllocater::Create()
{
	if (isCreated)return;
	if (!graphicsDevice)return;
	if (!heap)return;

	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC resDesc = {};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resDesc.Width = (UINT64)((1 + 0xff) & ~0xff) * (int)heap->GetUseCount().x;
	HRESULT result = graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer));

	buffer->Map(0, {}, (void**)&mappedBuffer);
	isCreated = true;
}

void GatesEngine::CBufferAllocater::ResetCurrentUseNumber()
{
	currentUseNumber = 0;
}

void GatesEngine::CBufferAllocater::BindAndAttachData(int descIndex, const void* data, int size)
{
	if (!heap)return;

	int sizeAligned = (size + 0xff) & ~0xff;
	int numRequired = sizeAligned / 0x100;

	if (currentUseNumber + numRequired > (int)heap->GetUseCount().x)return;

	int top = currentUseNumber;

	memcpy(mappedBuffer + top, data, size);

	D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
	cbDesc.BufferLocation = buffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
	cbDesc.SizeInBytes = sizeAligned;

	D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
	cpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;
	graphicsDevice->GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

	D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
	gpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;

	graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

	currentUseNumber += numRequired;
}
