#include "..\..\Header\Graphics\RootSignature.h"
#include "..\..\Header\Graphics\COMRelease.h"

void GatesEngine::RootSignature::CreateRange(D3D12_DESCRIPTOR_RANGE* range, RangeType type, int count)
{
	switch (type)
	{
	case RangeType::CBV:
		(*range) = {};
		(*range).NumDescriptors = 1;
		(*range).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_CBV;
		(*range).BaseShaderRegister = count;
		(*range).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	case RangeType::SRV:
		(*range) = {};
		(*range).NumDescriptors = 1;
		(*range).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		(*range).BaseShaderRegister = count;
		(*range).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	case RangeType::UAV:
		(*range) = {};
		(*range).NumDescriptors = 1;
		(*range).RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_UAV;
		(*range).BaseShaderRegister = count;
		(*range).OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		break;
	}
}

void GatesEngine::RootSignature::CreateSampler(D3D12_STATIC_SAMPLER_DESC& samplerDesc)
{
	samplerDesc = {};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MinLOD = 0.0f;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	samplerDesc.MaxAnisotropy = 16;
}

void GatesEngine::RootSignature::CreateStaticSampler(D3D12_STATIC_SAMPLER_DESC* pSamplerDesc, TextureAddressMode mode, D3D12Filter filter, int count)
{
	D3D12_TEXTURE_ADDRESS_MODE addressMode = mode == TextureAddressMode::WRAP ? D3D12_TEXTURE_ADDRESS_MODE_WRAP : D3D12_TEXTURE_ADDRESS_MODE_CLAMP;
	D3D12_FILTER samplingFilter = filter == D3D12Filter::POINT ? D3D12_FILTER_MIN_MAG_MIP_POINT : D3D12_FILTER_MIN_MAG_MIP_LINEAR;

	(*pSamplerDesc) = {};
	(*pSamplerDesc).AddressU = addressMode;
	(*pSamplerDesc).AddressV = addressMode;
	(*pSamplerDesc).AddressW = addressMode;
	(*pSamplerDesc).BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;
	(*pSamplerDesc).Filter = samplingFilter;
	(*pSamplerDesc).MaxLOD = D3D12_FLOAT32_MAX;
	(*pSamplerDesc).MinLOD = 0.0f;
	(*pSamplerDesc).ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	(*pSamplerDesc).ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	(*pSamplerDesc).MaxAnisotropy = 16;
	(*pSamplerDesc).ShaderRegister = count;
}

GatesEngine::RootSignature::RootSignature(GraphicsDevice* graphicsDevice, const std::vector<RangeType>& rangeTypes)
	:pGraphicsDevice(graphicsDevice)
	, rangeType(rangeTypes)
	, rootBlob(nullptr)
	, rootSignature(nullptr)
{
}

GatesEngine::RootSignature::~RootSignature()
{
	COM_RELEASE(rootSignature);
	COM_RELEASE(rootBlob);
}

void GatesEngine::RootSignature::Create()
{
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
	int rangeCount = (int)rangeType.size();
	//コンストラクタで指定されたレンジの数だけルートパラメータ,レンジを生成
	D3D12_ROOT_PARAMETER* rootParam = new D3D12_ROOT_PARAMETER[rangeCount];
	D3D12_DESCRIPTOR_RANGE* range = new D3D12_DESCRIPTOR_RANGE[rangeCount];
	//SRVの数だけSamplerDescを生成
	int samplerCount = 0;
	for (int i = 0; i < (int)rangeType.size(); ++i)
	{
		if (rangeType[i] == RangeType::SRV)
		{
			++samplerCount;
		}
	}
	D3D12_STATIC_SAMPLER_DESC* samplerDesc = new D3D12_STATIC_SAMPLER_DESC[samplerCount];
	//指定された順に設定
	int cbvCount = 0;
	samplerCount = 0;
	bool isSampler = false;
	int uavCount = 0;

	for (int i = 0; i < rangeCount; ++i)
	{
		switch (rangeType[i])
		{
		case RangeType::CBV:
			CreateRange(&range[i], RangeType::CBV, cbvCount);
			rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
			rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			rootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++cbvCount;
			break;
		case RangeType::SRV:
			CreateRange(&range[i], RangeType::SRV, samplerCount);
			rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
			rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			rootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++samplerCount;
			isSampler = true;
			break;
		case RangeType::UAV:
			CreateRange(&range[i], RangeType::UAV, uavCount);
			rootParam[i].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
			rootParam[i].DescriptorTable.pDescriptorRanges = &range[i];
			rootParam[i].DescriptorTable.NumDescriptorRanges = 1;
			rootParam[i].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
			++uavCount;
			break;
		}
	}

	D3D12_STATIC_SAMPLER_DESC* staticSamplerDesc = new D3D12_STATIC_SAMPLER_DESC[4];
	if (isSampler)
	{
		CreateStaticSampler(&staticSamplerDesc[0],TextureAddressMode::WRAP,D3D12Filter::POINT,0);
		CreateStaticSampler(&staticSamplerDesc[1],TextureAddressMode::CLAMP,D3D12Filter::POINT,1);
		CreateStaticSampler(&staticSamplerDesc[2],TextureAddressMode::WRAP,D3D12Filter::LINEAR,2);
		CreateStaticSampler(&staticSamplerDesc[3],TextureAddressMode::CLAMP,D3D12Filter::LINEAR,3);
	}
	rootSignatureDesc.pStaticSamplers = isSampler ? staticSamplerDesc : nullptr;
	rootSignatureDesc.NumStaticSamplers = isSampler ? 4 : 0;
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParam;
	rootSignatureDesc.NumParameters = (int)rangeType.size();

	ID3DBlob* errorBlob = nullptr;
	HRESULT result;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootBlob, &errorBlob);

	if (errorBlob)
	{
		OutputDebugStringA((char*)errorBlob->GetBufferPointer());
		errorBlob->Release();
	}

	result = pGraphicsDevice->GetDevice()->CreateRootSignature(0, rootBlob->GetBufferPointer(), rootBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
	delete[] rootParam;
	delete[] range;
	delete[] samplerDesc;
	delete[] staticSamplerDesc;
}

ID3D12RootSignature* GatesEngine::RootSignature::Get()
{
	return rootSignature;
}
