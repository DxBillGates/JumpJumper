#include "..\..\Header\Graphics\RenderTexture.h"
#include "..\..\Header\Graphics\DescriptorHeapManager.h"
#include "..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::RenderTexture::RenderTexture()
	:texBuff(nullptr)
	,srvNumber(0)
{
}

GatesEngine::RenderTexture::~RenderTexture()
{
	COM_RELEASE(texBuff);
}

void GatesEngine::RenderTexture::Prepare()
{
	//SRVからRTVに変更
	if (currentResourceState != D3D12_RESOURCE_STATE_RENDER_TARGET)
	{
		currentResourceState = D3D12_RESOURCE_STATE_RENDER_TARGET;
		pGraphicsDevice->SetResourceBarrier(texBuff, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void GatesEngine::RenderTexture::Create(GraphicsDevice* graphicsDevice, const GatesEngine::Math::Vector2& size)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = graphicsDevice->GetCurrentFrameBuffer()->GetDesc();
	resDesc.Width = (UINT)size.x;
	resDesc.Height = (UINT)size.y;

	float color[4] = {0,0,0,1};
	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.Color[0] = color[0];
	clearValue.Color[1] = color[1];
	clearValue.Color[2] = color[2];
	clearValue.Color[3] = color[3];

	graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&texBuff));

	//renderTarget.Create(graphicsDevice, texBuff);

	pResources.resize(1);
	//ヒープの生成
	HRESULT result;
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = 1;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	result = graphicsDevice->GetDevice()->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	//ビューの生成
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	D3D12_CPU_DESCRIPTOR_HANDLE rtvHeapHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	graphicsDevice->GetDevice()->CreateRenderTargetView(texBuff, &rtvDesc, rtvHeapHandle);
	pResources[0] = texBuff;
	//pGraphicsDevice = wrapper;

	srvNumber = graphicsDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	graphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(texBuff);
	//graphicsDevice->GetDescriptorHeapManager()->CreateSRV(texBuff);

	pGraphicsDevice = graphicsDevice;
}

void GatesEngine::RenderTexture::Set(int descIndex)
{
	if (currentResourceState == D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)EndDraw();
	pGraphicsDevice->GetCmdList()->SetGraphicsRootDescriptorTable(descIndex, pGraphicsDevice->GetCBVSRVUAVHeap()->GetSRVHandleForSRV(srvNumber));
}

void GatesEngine::RenderTexture::EndDraw()
{
	//RTVからSRVに変更してシェーダーにセット
	currentResourceState = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	pGraphicsDevice->SetResourceBarrier(texBuff, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
