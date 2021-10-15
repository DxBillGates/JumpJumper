#include "..\..\Header\Graphics\Shader.h"
#include "..\..\Header\Graphics\COMRelease.h"
#include <d3dcompiler.h>
#pragma comment(lib,"d3dcompiler.lib")

GatesEngine::Shader::Shader()
	: pGraphicsDevice(nullptr)
	, pipeline(nullptr)
	, rootSignature(nullptr)
	, vsBlob(nullptr)
	, psBlob(nullptr)
	, gsBlob(nullptr)
	, isCreate(false)
	, isCreatePipelineOrRootSignature(false)
	, blendMode(BlendMode::BLENDMODE_ADD)
	, topologyType(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE)
	, depthFlag(false)
	, rtvCount(0)
	, isSetGraphicsDevice(false)
	, isLoadShaderFile(false)
	, isSetInputLayout(false)
	, isSetRootParamerters(false)
	, isSetBlendMode(false)
	, isSetPrimitiveTopologyType(false)
	, isSetDepthFlag(false)
	, isSetRtvCount(false)
{
}

GatesEngine::Shader::Shader(GraphicsDevice* graphicsDevice, const std::wstring& fileName)
	: Shader()
{
	SetGraphicsDevice(graphicsDevice);
	LoadShaderFile(fileName);

	isLoadShaderFile = (!vsBlob || !psBlob) ? false : true;
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
	SetInputLayout(inputLayouts);
	SetRootParamerters(rangeTypes);
	SetBlendMode(blendMode);
	SetPrimitiveTopology(topologyType);
	SetIsUseDepth(depthFlag);
	SetRtvCount(rtvCount);

	CreatePipeline();
}

bool GatesEngine::Shader::SetGraphicsDevice(GraphicsDevice* graphicsDevice)
{
	isSetGraphicsDevice = (!graphicsDevice) ? false : true;
	pGraphicsDevice = graphicsDevice;
	return isSetGraphicsDevice;
}

bool GatesEngine::Shader::LoadShaderFile(const std::wstring& fileName)
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

	isLoadShaderFile = (!vsBlob || !psBlob) ? false : true;
	return isLoadShaderFile;
}

bool GatesEngine::Shader::SetInputLayout(const std::vector<InputLayout>& layouts)
{
	if ((int)layouts.size() == 0)
	{
		isSetInputLayout = false;
	}
	isSetInputLayout = true;
	inputLayouts = layouts;
	return isSetInputLayout;
}

bool GatesEngine::Shader::SetRootParamerters(const std::vector<RangeType>& ranges)
{
	if ((int)ranges.size() == 0)
	{
		isSetRootParamerters = false;
	}
	isSetRootParamerters = true;
	this->ranges = ranges;
	return isSetRootParamerters;
}

bool GatesEngine::Shader::SetBlendMode(BlendMode mode)
{
	blendMode = mode;
	isSetBlendMode = true;
	return isSetBlendMode;
}

bool GatesEngine::Shader::SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE type)
{
	topologyType = type;
	isSetPrimitiveTopologyType = true;
	return isSetPrimitiveTopologyType;
}

bool GatesEngine::Shader::SetIsUseDepth(bool depthFlag)
{
	this->depthFlag = depthFlag;
	isSetDepthFlag = true;
	return isSetDepthFlag;
}

bool GatesEngine::Shader::SetRtvCount(unsigned int value)
{
	isSetRtvCount = (value == 0) ? false : true;
	rtvCount = value;
	return isSetRtvCount;
}

bool GatesEngine::Shader::Check()
{
	bool isTrueReturn = true;
	if (!isSetGraphicsDevice)
	{
		printf("GraphicsDeviceがセットされていません\n");
		isTrueReturn = false;
	}
	if (!isLoadShaderFile)
	{
		printf("シェーダーファイルがロードされていません\n");
		isTrueReturn = false;
	}
	if (!isSetInputLayout)
	{
		printf("インプットレイアウトがセットされていません\n");
		isTrueReturn = false;
	}
	if (!isSetRootParamerters)
	{
		printf("ロートパラメータがセットされていません\n");
		isTrueReturn = false;
	}

	if (!isTrueReturn)
	{
		printf("グラフィックスパイプラインを生成できませんでした\n");
		return false;
	}

	if (!isSetBlendMode)
	{
		printf("ブレンドモードがセットされていません、アルファブレンドをセットします\n");
		SetBlendMode(BlendMode::BLENDMODE_ALPHA);
	}
	if (isSetPrimitiveTopologyType)
	{
		printf("トポロジータイプがセットされていません、三角形をセットします\n");
		SetPrimitiveTopology(D3D12_PRIMITIVE_TOPOLOGY_TYPE::D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
	}
	if (!isSetDepthFlag)
	{
		printf("深度フラグがセットされていません、深度フラグをtrueでセットします\n");
		SetIsUseDepth(true);
	}
	if (!isSetRtvCount)
	{
		printf("レンダ―ターゲットの数がセットされていません、1でセットします\n");
		SetRtvCount(1);
	}
	return true;
}

void GatesEngine::Shader::CreatePipeline()
{
	if (!Check())
	{
		return;
	}
	rootSignature = new RootSignature(pGraphicsDevice, ranges);
	rootSignature->Create();

	pipeline = new Pipeline(pGraphicsDevice, rootSignature, inputLayouts, blendMode, topologyType);
	pipeline->Create({ vsBlob,psBlob,gsBlob }, depthFlag, rtvCount);

	isCreate = true;
	isCreatePipelineOrRootSignature = true;
}

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
