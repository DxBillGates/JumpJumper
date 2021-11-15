#pragma once
#include "GraphicsDevice.h"

namespace GatesEngine
{
	class CBVSRVUAVHeap
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3D12DescriptorHeap* heap;
		Math::Vector3 useCount;
		bool isCreated;
		int incrementSize;
		int nextSrvDescriptorNum;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		CBVSRVUAVHeap();
		~CBVSRVUAVHeap();

		/// <summary>
		/// GraphicsDeviceのポインタをセットする関数
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDeviceのポインタ</param>
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);

		/// <summary>
		/// ヒープの生成関数
		/// </summary>
		/// <param name="useCount">CBV、SRV、UAVの順番でいくつのデータを格納できるようにするか設定する値</param>
		void Create(const Math::Vector3& useCount);

		/// <summary>
		/// ディスクリプタヒープのポインタを返す関数
		/// </summary>
		/// <returns>ディスクリプタヒープのポインタ</returns>
		ID3D12DescriptorHeap* GetHeap();

		/// <summary>
		/// CBV、SRV、UAVの順番でいくつのデータを格納できるか返す関数
		/// </summary>
		/// <returns>各データの許容値</returns>
		Math::Vector3 GetUseCount();

		/// <summary>
		/// 引数で渡されたGPUバッファからShaderResourceViewを生成する関数
		/// </summary>
		/// <param name="buffer">GPUBuffer</param>
		void CreateSRV(ID3D12Resource* buffer);

		/// <summary>
		/// 引数で渡されたGPUバッファと指定されたViewの生成データからShaderResourceViewを生成する関数
		/// </summary>
		/// <param name="buffer">GPUBuffer</param>
		/// <param name="viewDesc"></param>
		void CreateSRV(ID3D12Resource* buffer, const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

		/// <summary>
		/// SRVのGPU側アドレスを返す関数
		/// </summary>
		/// <param name="number">テクスチャ番号</param>
		/// <returns>SRVのGPU側アドレス</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForSRV(int number);

		/// <summary>
		/// 次に生成されるべきSRV用のディスクリプタヒープの番地を返す関数
		/// </summary>
		/// <returns>次に生成されるべきSRV用のディスクリプタヒープの番地</returns>
		int GetNextSrvNumber();

		/// <summary>
		/// ヒープをセットする関数
		/// </summary>
		void Set();
	};
}
