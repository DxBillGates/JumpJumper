#pragma once
#include "GraphicsDevice.h"
#include "CBVSRVUAVHeap.h"

namespace GatesEngine
{
	class CBufferAllocater
	{
	private:
		GraphicsDevice* graphicsDevice;
		CBVSRVUAVHeap* heap;
		ID3D12Resource* buffer;
		struct { char buf[256]; }*mappedBuffer;
		int currentUseNumber;
		bool isCreated;
	public:
		~CBufferAllocater();

		/// <summary>
		/// GraphicsDeviceのポインタをセットする関数
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// バカでかいディスクリプタヒープのポインタをセットする関数
		/// </summary>
		/// <param name="pHeap">ヒープのポインタ</param>
		void SetHeap(CBVSRVUAVHeap* pHeap);

		/// <summary>
		/// アロケータの生成関数
		/// </summary>
		void Create();

		/// <summary>
		/// 使用しているバッファの番号情報を初期化する関数
		/// </summary>
		void ResetCurrentUseNumber();

		/// <summary>
		/// セットするレジスタ番号、セットするデータのポインタ、データサイズを指定して
		/// これから使う定数バッファの番地の計算を行ったのちデータを
		/// シェーダにデータをセットする関数、主に配列データをセットしたいときに使用
		/// </summary>
		/// <param name="descIndex">レジスタ番号</param>
		/// <param name="data">データポインタ</param>
		/// <param name="size">データサイズ</param>
		void BindAndAttachData(int descIndex, const void* data, int size);

		/// <summary>
		/// セットするレジスタ番号、データを指定してこれから使う定数バッファの番地の計算を行ったのちデータをシェーダにセットする関数
		/// </summary>
		/// <typeparam name="T">セットするデータの型</typeparam>
		/// <param name="descIndex">レジスタ番号</param>
		/// <param name="data">データ</param>
		template<typename T>
		void BindAndAttach(int descIndex,const T& data);
	};
	template<typename T>
	inline void CBufferAllocater::BindAndAttach(int descIndex, const T& data)
	{
		if (!heap)return;

		//dataのサイズを256アライメントして計算
		int sizeAligned = (sizeof(T) + 0xff) & ~0xff;
		//256byteをいくつ使用するかアライメントした結果を256で割って計算
		int useValue = sizeAligned / 0x100;

		//現在使われ終わってる番号と今から使う容量がヒープの容量を超えている場合はリターン
		if (currentUseNumber + useValue > (int)heap->GetUseCount().x)return;

		int top = currentUseNumber;

		//先頭アドレスに使う分のポインタを足してmemcpy
		memcpy(mappedBuffer + top, &data, sizeof(T));

		//ビューを作って値をShaderにアタッチ
		D3D12_CONSTANT_BUFFER_VIEW_DESC cbDesc = {};
		cbDesc.BufferLocation = buffer->GetGPUVirtualAddress() + (UINT64)top * 0x100;
		cbDesc.SizeInBytes = sizeAligned;

		D3D12_CPU_DESCRIPTOR_HANDLE cpuHandle = heap->GetHeap()->GetCPUDescriptorHandleForHeapStart();
		cpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;
		graphicsDevice->GetDevice()->CreateConstantBufferView(&cbDesc, cpuHandle);

		D3D12_GPU_DESCRIPTOR_HANDLE gpuHandle = heap->GetHeap()->GetGPUDescriptorHandleForHeapStart();
		gpuHandle.ptr += (UINT64)graphicsDevice->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV) * currentUseNumber;

		graphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, gpuHandle);

		currentUseNumber += useValue;
	}
}