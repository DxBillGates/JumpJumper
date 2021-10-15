#pragma once
#include "GraphicsDevice.h"
#include "MeshData.h"
#include "VertexInfo.h"
#include <d3d12.h>

namespace GatesEngine
{
	class Mesh
	{
	private:
		ID3D12Resource* vBuffer;
		ID3D12Resource* iBuffer;
		D3D12_VERTEX_BUFFER_VIEW vbView;
		D3D12_INDEX_BUFFER_VIEW ibView;
		UINT indicesCount;

		GraphicsDevice* pGraphicsDevice;
		ID3D12GraphicsCommandList* cmdList;
	public:
		Mesh();
		~Mesh();
		template<typename T>
		void Create(GraphicsDevice* graphicsDevice,MeshData<T>& meshData);
		void Draw(UINT value = 1);
	};
	template<typename T>
	inline void Mesh::Create(GraphicsDevice* graphicsDevice, MeshData<T>& meshData)
	{
		using namespace GatesEngine::VertexInfo;
		pGraphicsDevice = graphicsDevice;
		ID3D12Device* device = graphicsDevice->GetDevice();
		cmdList = graphicsDevice->GetCmdList();
		std::vector<T>* vertices = meshData.GetVertices();
		std::vector<unsigned short>* indices = meshData.GetIndices();

		if ((int)vertices->size() == 0)return;

		D3D12_HEAP_PROPERTIES heapProp = {};
		heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC resDesc = {};
		resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
		resDesc.Height = 1;
		resDesc.DepthOrArraySize = 1;
		resDesc.MipLevels = 1;
		resDesc.SampleDesc.Count = 1;
		resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
		resDesc.Width = sizeof(T) * (*vertices).size();

		HRESULT result;
		result = device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&vBuffer));

		vbView.BufferLocation = vBuffer->GetGPUVirtualAddress();
		vbView.SizeInBytes = (UINT)resDesc.Width;
		vbView.StrideInBytes = sizeof(T);
		resDesc.Width = sizeof(unsigned short) * (*indices).size();
		device->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&iBuffer));
		ibView.BufferLocation = iBuffer->GetGPUVirtualAddress();
		ibView.SizeInBytes = (UINT)resDesc.Width;
		ibView.Format = DXGI_FORMAT_R16_UINT;

		T* vbMap = nullptr;
		unsigned short* ibMap = nullptr;
		vBuffer->Map(0, nullptr, (void**)&vbMap);
		iBuffer->Map(0, nullptr, (void**)&ibMap);
		for (int i = 0; i < (int)(*vertices).size(); ++i)
		{
			vbMap[i] = (*vertices)[i];
		}
		for (int i = 0; i < (int)(*indices).size(); ++i)
		{
			ibMap[i] = (*indices)[i];
		}
		vBuffer->Unmap(0, nullptr);
		iBuffer->Unmap(0, nullptr);

		indicesCount = (UINT)(*indices).size();
	}
}