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
		ID3DBlob* psBlob;
		ID3DBlob* gsBlob;

		bool isCreate;
		bool isCreatePipelineOrRootSignature;
	public:
		Shader(GraphicsDevice* graphicsDevice, const std::wstring& fileName);
		~Shader();
		void Create(const std::vector<InputLayout>& inputLayouts, const std::vector<RangeType>& rangeTypes,
			        BlendMode blendMode = BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE,bool depthFlag = true,int rtvCount = 1);
		//void Create(Pipeline* pPipeline, RootSignature* pRootSignature);
		void Set(bool wireFrame = false);
	};
}
