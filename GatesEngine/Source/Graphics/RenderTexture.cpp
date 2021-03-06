#include "..\..\Header\Graphics\RenderTexture.h"
#include "..\..\Header\Graphics\CBVSRVUAVHeap.h"
#include "..\..\Header\Graphics\COMRelease.h"

GatesEngine::RenderTexture::RenderTexture()
	: RenderTarget()
	, Texture()
{
}

GatesEngine::RenderTexture::~RenderTexture()
{
	COM_RELEASE(texBuffer);
}

void GatesEngine::RenderTexture::Prepare()
{
	//SRVからRTVに変更
	if (currentResourceState != D3D12_RESOURCE_STATE_RENDER_TARGET)
	{
		currentResourceState = D3D12_RESOURCE_STATE_RENDER_TARGET;
		pGraphicsDevice->SetResourceBarrier(texBuffer, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, D3D12_RESOURCE_STATE_RENDER_TARGET);
	}
}

void GatesEngine::RenderTexture::Create(GraphicsDevice* graphicsDevice, const GatesEngine::Math::Vector2& size, const Math::Vector4& color)
{
	D3D12_HEAP_PROPERTIES heapProp = {};
	heapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	D3D12_RESOURCE_DESC resDesc = graphicsDevice->GetCurrentFrameBuffer()->GetDesc();
	resDesc.Width = (UINT)size.x;
	resDesc.Height = (UINT)size.y;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	clearValue.Color[0] = (color.x > 1) ? color.x / 255.0f : color.x;
	clearValue.Color[1] = (color.y > 1) ? color.y / 255.0f : color.y;
	clearValue.Color[2] = (color.z > 1) ? color.z / 255.0f : color.z;
	clearValue.Color[3] = (color.w > 1) ? color.w / 255.0f : color.w;

	graphicsDevice->GetDevice()->CreateCommittedResource(&heapProp, D3D12_HEAP_FLAG_NONE, &resDesc, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE, &clearValue, IID_PPV_ARGS(&texBuffer));

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
	graphicsDevice->GetDevice()->CreateRenderTargetView(texBuffer, &rtvDesc, rtvHeapHandle);
	pResources[0] = texBuffer;
	//pGraphicsDevice = wrapper;

	srvNumber = graphicsDevice->GetCBVSRVUAVHeap()->GetNextSrvNumber();
	graphicsDevice->GetCBVSRVUAVHeap()->CreateSRV(texBuffer);
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
	pGraphicsDevice->SetResourceBarrier(texBuffer, D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE);
}
