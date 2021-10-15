#include "..\..\Header\Graphics\ComputePipeline.h"
#include "..\..\Header\Graphics\COMRelease.h"
#include <d3dcompiler.h>

GatesEngine::ComputePipeline::ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filename)
	: graphicsDevice(pGraphicsDevice)
	, shaderBlob(nullptr)
	, pipeline(nullptr)
	, rootSignature(nullptr)
{
	//CSのコンパイル＆生成
	std::wstring filepath = L"Resources/Shader/" + filename + L"CS" + L".hlsl"; 
	auto hr = D3DCompileFromFile(filepath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, "main", "cs_5_1", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &shaderBlob, nullptr);
}

GatesEngine::ComputePipeline::~ComputePipeline()
{
	COM_RELEASE(shaderBlob);
	COM_RELEASE(pipeline);
	delete rootSignature;
}

void GatesEngine::ComputePipeline::Create(const std::vector<RangeType>& range)
{
	//ルートシグネチャの生成
	rootSignature = new GatesEngine::RootSignature(graphicsDevice, range);
	rootSignature->Create();

	D3D12_COMPUTE_PIPELINE_STATE_DESC pipeDesc = {};
	pipeDesc.CS.pShaderBytecode = shaderBlob->GetBufferPointer();
	pipeDesc.CS.BytecodeLength = shaderBlob->GetBufferSize();
	pipeDesc.Flags = D3D12_PIPELINE_STATE_FLAG_NONE;
	pipeDesc.NodeMask = 0;
	pipeDesc.pRootSignature = rootSignature->Get();

	HRESULT hr = graphicsDevice->GetDevice()->CreateComputePipelineState(&pipeDesc, IID_PPV_ARGS(&pipeline));
}

void GatesEngine::ComputePipeline::Set()
{
	graphicsDevice->GetCmdList()->SetComputeRootSignature(rootSignature->Get());
	graphicsDevice->GetCmdList()->SetPipelineState(pipeline);
}
