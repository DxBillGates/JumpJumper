#include "..\..\Header\Graphics\SamplerHeap.h"
#include "..\..\Header\Graphics\COMRelease.h"

void GatesEngine::SamplerHeap::CreateSampler(const D3D12_SAMPLER_DESC& samplderDesc,int count)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handle = heap->GetCPUDescriptorHandleForHeapStart();
	handle.ptr += (int64_t)count * incrementSize;
	graphicsDevice->GetDevice()->CreateSampler(&samplderDesc, handle);
}

GatesEngine::SamplerHeap::SamplerHeap()
{
}

GatesEngine::SamplerHeap::~SamplerHeap()
{
	COM_RELEASE(heap);
}

void GatesEngine::SamplerHeap::SetGraphicsDevice(GraphicsDevice* gDevice)
{
	graphicsDevice = gDevice;
}

void GatesEngine::SamplerHeap::Create()
{
	if (!graphicsDevice)return;

	incrementSize = graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER);

	D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
	heapDesc.NumDescriptors = 4;
	heapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	heapDesc.NodeMask = 0;
	heapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;

	graphicsDevice->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	for (int i = 0; i < 4; ++i)
	{
		samplerDesc.BorderColor[i] = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	}
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.MaxAnisotropy = 16;

	CreateSampler(samplerDesc, 0);

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	CreateSampler(samplerDesc, 1);

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	CreateSampler(samplerDesc, 2);

	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	CreateSampler(samplerDesc, 3);
}
