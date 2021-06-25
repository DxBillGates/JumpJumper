#pragma once
#include "GraphicsDevice.h"
#include "DescriptorHeapManager.h"
#include "COMRelease.h"
#include "CBufferStruct.h"
namespace GatesEngine
{
	template<typename T>
	class CBuffer
	{
	private:
		GraphicsDevice* graphicsDevice;
		DescriptorHeapManager* pHeap;
		int rangeNumber;
		int cbvNumber;				//ヒープの何番目にあるか
		ID3D12Resource* buffer;		//定数バッファ
		bool isCreate;
	public:
		CBuffer();
		~CBuffer();
		void Create(GraphicsDevice* pDevice, int rangeNumber);
		void Map(const T& mappinData);
		int GetCbvNumber();
		int GetRangeNumber();
		void Set();
	};

	template<typename T>
	inline CBuffer<T>::CBuffer()
		: graphicsDevice(nullptr)
		, pHeap(nullptr)
		, rangeNumber(0)
		, cbvNumber(0)
		, buffer(nullptr)
		, isCreate(false)
	{
	}

	template<typename T>
	inline CBuffer<T>::~CBuffer()
	{
		COM_RELEASE(buffer);
	}

	template<typename T>
	inline void CBuffer<T>::Create(GraphicsDevice* pDevice, int rangeNumber)
	{
		if (isCreate)
		{
			return;
		}
		else
		{
			graphicsDevice = pDevice;
			pHeap = pDevice->GetDescriptorHeapManager();
			cbvNumber = pHeap->GetNextCbvDescriptorNumber();
			this->rangeNumber = rangeNumber;
			//定数バッファの設定
			D3D12_HEAP_PROPERTIES heapProp = {};
			heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
			D3D12_RESOURCE_DESC resDesc = {};
			resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
			resDesc.Height = 1;
			resDesc.DepthOrArraySize = 1;
			resDesc.MipLevels = 1;
			resDesc.SampleDesc.Count = 1;
			resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
			resDesc.Width = (sizeof(T) + 0xff) & ~0xff;
			HRESULT result = pDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&buffer));
			//定数バッファビューの設定
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = buffer->GetGPUVirtualAddress();
			cbvDesc.SizeInBytes = (UINT)resDesc.Width;
			//ヒープにビューを生成
			pHeap->CreateCBV(buffer);
			isCreate = true;
		}
	}

	template<typename T>
	inline void CBuffer<T>::Map(const T& mappinData)
	{
		if (!isCreate)
		{
			return;
		}
		else
		{
			//定数バッファのデータを書き換える
			T* cbMap = nullptr;
			buffer->Map(0, nullptr, (void**)&cbMap);
			*cbMap = mappinData;
			buffer->Unmap(0, nullptr);
		}
	}


	template<typename T>
	inline int CBuffer<T>::GetCbvNumber()
	{
		return cbvNumber;
	}

	template<typename T>
	inline int CBuffer<T>::GetRangeNumber()
	{
		return rangeNumber;
	}

	template<typename T>
	inline void CBuffer<T>::Set()
	{
		if (!isCreate)
		{
			return;
		}
		else
		{
			//pHeap->Set();
			graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(rangeNumber, pHeap->GetCBVHandleForGPU(cbvNumber));
		}
	}


}