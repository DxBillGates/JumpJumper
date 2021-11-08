#include "..\..\Header\Graphics\Pipeline.h"
#include "..\..\Header\Graphics\COMRelease.h"

void GatesEngine::Pipeline::SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* blendDesc, BlendMode mode)
{
	(*blendDesc).RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	(*blendDesc).BlendEnable = true;
	(*blendDesc).BlendOpAlpha = D3D12_BLEND_OP_ADD;
	(*blendDesc).SrcBlendAlpha = D3D12_BLEND_ONE;
	(*blendDesc).DestBlendAlpha = D3D12_BLEND_ZERO;
	switch (mode)
	{
	case BlendMode::BLENDMODE_ADD:
		/* 加算合成 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::BLENDMODE_SUB:
		/* 減算合成 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;
		(*blendDesc).SrcBlend = D3D12_BLEND_ONE;
		(*blendDesc).DestBlend = D3D12_BLEND_ONE;
		break;
	case BlendMode::BLENDMODE_INV:
		/* 色反転 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_INV_DEST_COLOR;
		(*blendDesc).DestBlend = D3D12_BLEND_ZERO;
		break;
	case BlendMode::BLENDMODE_ALPHA:
		/* 半透明 */
		(*blendDesc).BlendOp = D3D12_BLEND_OP_ADD;
		(*blendDesc).SrcBlend = D3D12_BLEND_SRC_ALPHA;
		(*blendDesc).DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
		break;
	}
}

GatesEngine::Pipeline::Pipeline(GraphicsDevice* graphicsDevice, RootSignature* rootSignature, const std::vector<InputLayout>& inputLayouts, BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType)
	:pGraphicsDevice(graphicsDevice)
	,rootSignature(rootSignature)
	, inputLayout(inputLayouts)
	, blendMode(blendMode)
	, topologyType(topologyType)
	, solidPso(nullptr)
	, wirePso(nullptr)
{
}
GatesEngine::Pipeline::~Pipeline()
{
	delete rootSignature;
	COM_RELEASE(solidPso);
	COM_RELEASE(wirePso);
}

void GatesEngine::Pipeline::Create(std::vector<ID3DBlob*> blobs, bool depthFlag, int rtvCount)
{
	D3D12_INPUT_ELEMENT_DESC* inputDesc = new D3D12_INPUT_ELEMENT_DESC[(int)inputLayout.size()];
	//引数のインプットレイアウトからPSOのインプットレイアウトを設定
	for (int i = 0; i < (int)inputLayout.size(); ++i)
	{
		if (inputLayout[i] == InputLayout::POSITION)
		{
			inputDesc[i] = { "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (inputLayout[i] == InputLayout::TEXCOORD)
		{
			inputDesc[i] = { "TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D12_APPEND_ALIGNED_ELEMENT,D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0 };
		}
		else if (inputLayout[i] == InputLayout::NORMAL)
		{
			inputDesc[i] = { "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
		else if (inputLayout[i] == InputLayout::COLOR)
		{
			inputDesc[i] = { "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D12_APPEND_ALIGNED_ELEMENT, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 };
		}
	}
	//ブレンド設定
	D3D12_RENDER_TARGET_BLEND_DESC blendDesc = {};
	SetBlendMode(&blendDesc, BlendMode::BLENDMODE_ALPHA);
	//PSOの各種設定
	HRESULT result;
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.VS.BytecodeLength = blobs[0]->GetBufferSize();
	psoDesc.VS.pShaderBytecode = blobs[0]->GetBufferPointer();
	psoDesc.PS.BytecodeLength = blobs[1]->GetBufferSize();
	psoDesc.PS.pShaderBytecode = blobs[1]->GetBufferPointer();
	if (blobs[2])
	{
		psoDesc.GS.BytecodeLength = blobs[2]->GetBufferSize();
		psoDesc.GS.pShaderBytecode = blobs[2]->GetBufferPointer();
	}
	if (blobs[3])
	{
		psoDesc.HS.BytecodeLength = blobs[3]->GetBufferSize();
		psoDesc.HS.pShaderBytecode = blobs[3]->GetBufferPointer();
	}
	if (blobs[4])
	{
		psoDesc.DS.BytecodeLength = blobs[4]->GetBufferSize();
		psoDesc.DS.pShaderBytecode = blobs[4]->GetBufferPointer();
	}

	psoDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	psoDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK;
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	if (depthFlag)
	{
		psoDesc.RasterizerState.DepthClipEnable = true;
		psoDesc.DepthStencilState.DepthEnable = true;
		psoDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		psoDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
		psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	}
	else
	{
		psoDesc.DepthStencilState.DepthEnable = FALSE;
		psoDesc.DepthStencilState.StencilEnable = FALSE;
	}
	psoDesc.BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
	psoDesc.BlendState.RenderTarget[0] = blendDesc;
	psoDesc.InputLayout.pInputElementDescs = inputDesc;
	psoDesc.InputLayout.NumElements = (int)inputLayout.size();
	psoDesc.PrimitiveTopologyType = (blobs[3] && blobs[4]) ? D3D12_PRIMITIVE_TOPOLOGY_TYPE_PATCH : topologyType;
	psoDesc.NumRenderTargets = rtvCount;
	//psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;

	for (int i = 0; i < rtvCount; ++i)
	{
		psoDesc.RTVFormats[i] = DXGI_FORMAT_R8G8B8A8_UNORM;
	}

	psoDesc.SampleDesc.Count = 1;
	psoDesc.pRootSignature = rootSignature->Get();
	result = pGraphicsDevice->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&solidPso));
	psoDesc.RasterizerState.FillMode = D3D12_FILL_MODE_WIREFRAME;
	result = pGraphicsDevice->GetDevice()->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&wirePso));

	delete[] inputDesc;
}

ID3D12PipelineState* GatesEngine::Pipeline::GetSolidPso()
{
	return solidPso;
}

ID3D12PipelineState* GatesEngine::Pipeline::GetWirePso()
{
	return wirePso;
}

D3D12_PRIMITIVE_TOPOLOGY_TYPE GatesEngine::Pipeline::GetTopologyType()
{
	return topologyType;
}

