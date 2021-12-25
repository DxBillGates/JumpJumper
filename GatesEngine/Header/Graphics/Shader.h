#pragma once
#include "GraphicsDevice.h"
#include "RootSignature.h"
#include "Pipeline.h"
#include <string>
#include <vector>

namespace GatesEngine
{
	class Shader
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		Pipeline* pipeline;
		RootSignature* rootSignature;

		ID3DBlob* vsBlob;
		ID3DBlob* hsBlob;
		ID3DBlob* dsBlob;
		ID3DBlob* gsBlob;
		ID3DBlob* psBlob;

		bool isCreate;
		bool isCreatePipelineOrRootSignature;

		std::vector<InputLayout> inputLayouts;
		std::vector<RangeType> ranges;
		BlendMode blendMode;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType;
		bool depthFlag;
		unsigned int rtvCount;

		bool isSetGraphicsDevice;
		bool isLoadShaderFile;
		bool isSetInputLayout;
		bool isSetRootParamerters;
		bool isSetBlendMode;
		bool isSetPrimitiveTopologyType;
		bool isSetDepthFlag;
		bool isSetRtvCount;
		bool isSetAlphaToCoverageEnable;
	public:
		Shader();
		Shader(GraphicsDevice* graphicsDevice, const std::wstring& fileName);
		~Shader();
		void Create(const std::vector<InputLayout>& inputLayouts, const std::vector<RangeType>& rangeTypes,
			        BlendMode blendMode = BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,bool depthFlag = true,int rtvCount = 1,bool alphaToCovarage = false);
		void Create();

		bool SetGraphicsDevice(GraphicsDevice* graphicsDevice);
		bool LoadShaderFile(const std::wstring& fileName);
		bool SetInputLayout(const std::vector<InputLayout>& layouts);
		bool SetRootParamerters(const std::vector<RangeType>& ranges);
		bool SetBlendMode(BlendMode mode);
		bool SetPrimitiveTopology(PrimiriveTopologyType type);
		bool SetIsUseDepth(bool depthFlag);
		bool SetRtvCount(unsigned int value);
		bool SetAlphaToCoverageEnable(bool flag);
		bool Check();
		void CreatePipeline();

		void Set(bool wireFrame = false);
	};
}
