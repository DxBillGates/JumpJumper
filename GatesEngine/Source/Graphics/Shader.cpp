#include "..\..\Header\Graphics\Shader.h"
#include "..\..\Header\Graphics\COMRelease.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

GatesEngine::Shader::Shader(GraphicsDevice* graphicsDevice, const std::wstring& fileName)
	:pGraphicsDevice(graphicsDevice)
	,pipeline(nullptr)
	,rootSignature(nullptr)
	,vsBlob(nullptr)
	,psBlob(nullptr)
	,gsBlob(nullptr)
	,isCreate(false)
	,isCreatePipelineOrRootSignature(false)
{
	ID3DInclude* include = D3D_COMPILE_STANDARD_FILE_INCLUDE;
	UINT flag = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
	ID3DBlob* errorBlob = nullptr;
	HRESULT result;

	std::wstring format = L".hlsl";
	std::wstring firstPass = L"Resources/Shader/";

	std::wstring fullFilename = firstPass + fileName + L"VS" + format;
	result = D3DCompileFromFile(fullFilename.c_str(), nullptr, include, "main", "vs_5_0", flag, 0, &vsBlob, &errorBlob);

	fullFilename = firstPass + fileName + L"PS" + format;
	result = D3DCompileFromFile(fullFilename.c_str(), nullptr, include, "main", "ps_5_0", flag, 0, &psBlob, &errorBlob);

	fullFilename = firstPass + fileName + L"GS" + format;
	result = D3DCompileFromFile(fullFilename.c_str(), nullptr, include, "main", "gs_5_0", flag, 0, &gsBlob, &errorBlob);
	COM_RELEASE(errorBlob);
}

GatesEngine::Shader::~Shader()
{
	COM_RELEASE(vsBlob);
	COM_RELEASE(psBlob);
	COM_RELEASE(gsBlob);
	if (isCreatePipelineOrRootSignature)
	{
		delete pipeline;
	}
}

void GatesEngine::Shader::Create(const std::vector<InputLayout>& inputLayouts, const std::vector<RangeType>& rangeTypes, BlendMode blendMode, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType, bool depthFlag, int rtvCount)
{
	rootSignature = new RootSignature(pGraphicsDevice, rangeTypes);
	rootSignature->Create();

	pipeline = new Pipeline(pGraphicsDevice, rootSignature, inputLayouts,blendMode,topologyType);
	pipeline->Create({vsBlob,psBlob,gsBlob},depthFlag,rtvCount);

	isCreate = true;
	isCreatePipelineOrRootSignature = true;
}

//void GatesEngine::Shader::Create(Pipeline* pPipeline, RootSignature* pRootSignature)
//{
//}

void GatesEngine::Shader::Set(bool wireFrame)
{
	//引数のFILLMODEによってワイヤーフレームか塗りつぶしか分岐
	if (wireFrame)
	{
		pGraphicsDevice->GetCmdList()->SetPipelineState(pipeline->GetWirePso());
	}
	else
	{
		pGraphicsDevice->GetCmdList()->SetPipelineState(pipeline->GetSolidPso());
	}
	//ルートシグネチャのセット
	pGraphicsDevice->GetCmdList()->SetGraphicsRootSignature(rootSignature->Get());
}
