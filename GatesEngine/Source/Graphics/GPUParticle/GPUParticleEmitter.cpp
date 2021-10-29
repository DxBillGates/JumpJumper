#include "..\..\..\Header\Graphics\GPUParticle\GPUParticleEmitter.h"
#include "..\..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\..\Header\Graphics\COMRelease.h"
#include "..\..\..\Header\Graphics\Graphics.h"
#include "..\..\..\Header\Graphics\Manager\ResourceManager.h"
#include <vector>

GatesEngine::GPUParticleEmitter::GPUParticleEmitter()
	: manager(nullptr)
	, particleData(nullptr)
	, updateParticleData(nullptr)
	, useParticleOffset(0)
	, useParticleValue(0)
	, uavValue(0)
	, srvValue(0)
	, addDataSrvValue(0)
	, addBuffer(nullptr)
	, addData(nullptr)
{
}

GatesEngine::GPUParticleEmitter::~GPUParticleEmitter()
{
	COM_RELEASE(addBuffer);
}

void GatesEngine::GPUParticleEmitter::Update()
{
	//std::vector<ParticleData> date(useParticleValue);
	//ComputeShaderでUpdateしたパーティクルデータを先頭アドレスずらしてSRVのバッファにコピー
	memcpy(updateParticleData + useParticleOffset, particleData + useParticleOffset, sizeof(ParticleData) * useParticleValue);
	//date.assign((ParticleData*)updateParticleData + useParticleOffset, (ParticleData*)updateParticleData + useParticleOffset + useParticleValue);
}

void GatesEngine::GPUParticleEmitter::Draw(Camera* camera, ComputePipeline* computeShader, const Math::Vector3& pos)
{
	addData[0].pos = { pos.x,pos.y,pos.z,0 };

	GraphicsDevice* graphicsDevice = manager->GetDevice();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();
	ResourceManager::GetShaderManager()->GetShader("PointShader")->Set();
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ pos }));
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, camera->GetData());
	graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(3, graphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(srvValue));
	ResourceManager::GetMeshManager()->GetMesh("Point")->Draw(useParticleValue);

	ID3D12DescriptorHeap* heap = manager->GetHeap();
	computeShader->Set();
	manager->GetDevice()->GetCmdList()->SetDescriptorHeaps(1, &heap);

	auto handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)manager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * uavValue;
	manager->GetDevice()->GetCmdList()->SetComputeRootDescriptorTable(0, handle);
	handle = heap->GetGPUDescriptorHandleForHeapStart();
	handle.ptr += (UINT64)manager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * 200 + (UINT64)manager->GetDevice()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * addDataSrvValue;
	manager->GetDevice()->GetCmdList()->SetComputeRootDescriptorTable(1, handle);

	manager->GetDevice()->GetCmdList()->Dispatch(useParticleValue/128, 1, 1);
}

void GatesEngine::GPUParticleEmitter::Create(GPUParticleManager* manager, UINT useParticleValue)
{
	this->useParticleValue = useParticleValue;

	//ビューの生成
	D3D12_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
	uavDesc.ViewDimension = D3D12_UAV_DIMENSION::D3D12_UAV_DIMENSION_BUFFER;
	uavDesc.Format = DXGI_FORMAT_UNKNOWN;
	uavDesc.Buffer.FirstElement = manager->GetNextParticleOffset();
	uavDesc.Buffer.NumElements = useParticleValue;
	uavDesc.Buffer.StructureByteStride = sizeof(ParticleData);

	uavValue = manager->GetUavNextOffset();
	manager->CreateUAV(uavDesc);
	useParticleOffset = manager->GetNextParticleOffset();
	manager->IncrementParticleOffset(useParticleValue);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.Format = DXGI_FORMAT_UNKNOWN;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	srvDesc.Buffer.FirstElement = uavDesc.Buffer.FirstElement;
	srvDesc.Buffer.NumElements = useParticleValue;
	srvDesc.Buffer.StructureByteStride = sizeof(ParticleData);
	srvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	
	srvValue = manager->GetDevice()->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	manager->GetDevice()->GetCBVSRVUAVHeap()->CreateSRV(manager->GetUpdateParticleBuffer(), srvDesc);

	//加算用データ作成
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
	resDesc.Width = sizeof(ParticleData);
	manager->GetDevice()->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&addBuffer));

	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;
	//srvDesc.Buffer.StructureByteStride = sizeof(GatesEngine::Math::Vector4);
	addDataSrvValue = manager->GetSrvNextOffset();
	manager->CreateSRV(addBuffer,srvDesc);

	manager->GetParticleBuffer()->Map(0, nullptr, (void**)&particleData);
	manager->GetUpdateParticleBuffer()->Map(0, nullptr, (void**)&updateParticleData);
	addBuffer->Map(0, nullptr, (void**)&addData);

	addData[0].pos = {100,0,0,0};
	addData[0].vel = {1,0,0,0};

	//std::vector<ParticleData> date(useParticleValue);

	//先頭アドレスを進める
	for (int i = 0; i < (int)useParticleValue; ++i)
	{
		particleData[useParticleOffset + i].vel = { (float)std::rand() / RAND_MAX * 100 - 100 / 2.0f,-(float)(rand() % 5),(float)std::rand() / RAND_MAX * 100 - 100 / 2.0f,1 };
		particleData[useParticleOffset + i].vel = particleData[useParticleOffset + i].vel.Normalize();
		particleData[useParticleOffset + i].vel.w = 0;
	}

	//memcpy(updateParticleData + useParticleOffset, particleData + useParticleOffset, sizeof(ParticleData) * useParticleValue);

	//date.assign((ParticleData*)updateParticleData + useParticleOffset, (ParticleData*)updateParticleData + useParticleOffset + useParticleValue);

	this->manager = manager;
}
