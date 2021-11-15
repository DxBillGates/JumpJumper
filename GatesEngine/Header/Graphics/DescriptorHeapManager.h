#pragma once
#include "GraphicsDevice.h"
#include<d3d12.h>
namespace GatesEngine
{
	class DescriptorHeapManager
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		ID3D12DescriptorHeap* mHeap;
		int srvStartOffset;
		int incrementSize;
		int nextCbvDescriptorNum;
		int nextSrvDescriptorNum;
	public:
		/// <summary>
		/// コンストラクタ
		/// </summary>
		/// <param name="graphicsDevice">GraphicsDeviceのポインタ</param>
		/// <param name="cbvNum">CBVとして扱える許容値</param>
		/// <param name="srvNum">SRVとして扱える許容値</param>
		/// <param name="uavNum">UAVとして扱える許容値</param>
		DescriptorHeapManager(GraphicsDevice* graphicsDevice, int cbvNum, int srvNum, int uavNum = 0);
		~DescriptorHeapManager();

		/// <summary>
		/// 定数バッファビューの生成関数
		/// </summary>
		/// <param name="cBuff">定数バッファ</param>
		void CreateCBV(ID3D12Resource* cBuff);

		/// <summary>
		/// シェーダーリソースビューの生成関数
		/// </summary>
		/// <param name="texBuff">テクスチャバッファ</param>
		void CreateSRV(ID3D12Resource* texBuff);

		/// <summary>
		/// DescriptorHeapのセット関数
		/// </summary>
		void Set();

		/// <summary>
		/// DescriptorHeapのポインタを返す関数
		/// </summary>
		/// <returns>DescriptorHeapのポインタ</returns>
		ID3D12DescriptorHeap* Get();

		/// <summary>
		/// 引数の番号からDescriptorHeapのどこの番地に登録されているか返す関数(CBV:CPU版)
		/// </summary>
		/// <param name="number">番号</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetCBVHandleForCPU(int number);

		/// <summary>
		/// 引数の番号からDescriptorHeapのどこの番地に登録されているか返す関数(CBV:GPU版)
		/// </summary>
		/// <param name="number">番号</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetCBVHandleForGPU(int number);

		/// <summary>
		/// 引数の番号からDescriptorHeapのどこの番地に登録されているか返す関数(SRV:CPU版)
		/// </summary>
		/// <param name="number">番号</param>
		/// <returnescriptorHandle</returns>
		D3D12_CPU_DESCRIPTOR_HANDLE GetSRVHandleForCPU(int number);

		/// <summary>
		/// 引数の番号からDescriptorHeapのどこの番地に登録されているか返す関数(SRVGPU版)
		/// </summary>
		/// <param name="number">番号</param>
		/// <returns>DescriptorHandle</returns>
		D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleForGPU(int number);

		/// <summary>
		/// 次に登録できるCBV用の番号を返す関数
		/// </summary>
		/// <returns>次に登録できるCBV用の番号</returns>
		int GetNextCbvDescriptorNumber();

		/// <summary>
		/// 次に登録できるSRV用の番号を返す関数
		/// </summary>
		/// <returns>次に登録できるSRV用の番号</returns>
		int GetNextSrvDescriptorNumber();
	};


}