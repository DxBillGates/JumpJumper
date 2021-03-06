#include "..\..\..\Header\Graphics\GPUParticle\GPUParticleEmitter.h"
#include "..\..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\..\Header\Graphics\COMRelease.h"
#include "..\..\..\Header\Graphics\Graphics.h"
#include "..\..\..\Header\Graphics\Manager\ResourceManager.h"
#include "..\..\..\Header\Util\Random.h"
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

void GatesEngine::GPUParticleEmitter::Update(int addParticleValue, float addCyclePerFrame)
{
	bool addFlag = false;
	if (cycle > addCyclePerFrame)
	{
		addFlag = true;
		cycle = 0;
	}

	if (addFlag)
	{
		int count = 0;
		for (int i = 0; i < (int)useParticleValue; ++i)
		{
			if (count >= addParticleValue)break;

			if (particleData[useParticleOffset + i].isDead)
			{
				particleData[useParticleOffset + i].state = 0;
				++count;
			}
			else
			{
				continue;
			}
		}
	}

	cycle++;
}

void GatesEngine::GPUParticleEmitter::Draw(Camera* camera, ComputePipeline* computeShader, const Math::Vector3& pos, Texture* tex)
{
	addData[0].pos = { pos.x,pos.y,pos.z };
	//addData[0].vel = addVel;

	GraphicsDevice* graphicsDevice = manager->GetDevice();
	graphicsDevice->GetCBVSRVUAVHeap()->Set();
	ResourceManager::GetShaderManager()->GetShader("PointShader")->Set();
	graphicsDevice->GetCmdList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Translate({ 0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, camera->GetData());
	graphicsDevice->GetMainCamera()->Set(2);
	graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(3, graphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(srvValue));

	if (!tex)
	{
		ResourceManager::GetTextureManager()->GetTexture("particleTex")->Set(4);
	}
	else
	{
		tex->Set(4);
	}
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

	manager->GetDevice()->GetCmdList()->Dispatch(useParticleValue / 256, 1, 1);
}

void GatesEngine::GPUParticleEmitter::Create(GPUParticleManager* manager, UINT useParticleValue)
{
	this->useParticleValue = useParticleValue;

	//?r???[??????
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

	//???Z?p?f?[?^????
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
	resDesc.Width = sizeof(EmitterData);
	manager->GetDevice()->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&addBuffer));

	srvDesc.Buffer.FirstElement = 0;
	srvDesc.Buffer.NumElements = 1;
	srvDesc.Buffer.StructureByteStride = sizeof(EmitterData);
	addDataSrvValue = manager->GetSrvNextOffset();
	manager->CreateSRV(addBuffer, srvDesc);

	manager->GetParticleBuffer()->Map(0, nullptr, (void**)&particleData);
	manager->GetUpdateParticleBuffer()->Map(0, nullptr, (void**)&updateParticleData);
	addBuffer->Map(0, nullptr, (void**)&addData);

	addData[0].pos = { 0,0,0 };
	addData[0].vel = { 0,0,0 };
	addData[0].MAX_LIFE = 1;
	addData[0].startForceMin = -1;
	addData[0].startForceMax = 1;
	addData[0].startForce = { 1,1,1 };

	//std::vector<ParticleData> date(useParticleValue);

	//?????A?h???X???i????
	for (int i = 0; i < (int)useParticleValue; ++i)
	{
		particleData[useParticleOffset + i].pos = { 1000000,0,0,0 };
		particleData[useParticleOffset + i].vel = { (float)std::rand() / RAND_MAX * 100 - 100 / 2.0f,-(float)(rand() % 5),(float)std::rand() / RAND_MAX * 100 - 100 / 2.0f,1 };
		particleData[useParticleOffset + i].vel = particleData[useParticleOffset + i].vel.Normalize();
		particleData[useParticleOffset + i].vel.w = GatesEngine::Random::Rand(0, 1);
		particleData[useParticleOffset + i].isDead = true;
		particleData[useParticleOffset + i].state = 3;
	}

	//memcpy(updateParticleData + useParticleOffset, particleData + useParticleOffset, sizeof(ParticleData) * useParticleValue);

	//date.assign((ParticleData*)updateParticleData + useParticleOffset, (ParticleData*)updateParticleData + useParticleOffset + useParticleValue);

	this->manager = manager;
}

GatesEngine::EmitterData* GatesEngine::GPUParticleEmitter::GetAddData()
{
	return addData;
}
