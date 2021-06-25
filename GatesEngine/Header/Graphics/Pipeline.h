#pragma once
#include "GraphicsDevice.h"
#include "RootSignature.h"
namespace GatesEngine
{
	enum class BlendMode
	{
		BLENDMODE_ADD,
		BLENDMODE_SUB,
		BLENDMODE_INV,
		BLENDMODE_ALPHA
	};

	enum class InputLayout
	{
		POSITION,
		TEXCOORD,
		NORMAL,
		COLOR,
	};

	class Pipeline
	{
	private:
		GraphicsDevice* pGraphicsDevice;
		RootSignature* rootSignature;
		std::vector<InputLayout> inputLayout;

		BlendMode blendMode;
		D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType;

		ID3D12PipelineState* solidPso;
		ID3D12PipelineState* wirePso;
	private:
		void SetBlendMode(D3D12_RENDER_TARGET_BLEND_DESC* blendDesc, BlendMode mode);
	public:
		Pipeline(GraphicsDevice* graphicsDevice,RootSignature* rootSignature, const std::vector<InputLayout>& inputLayouts, 
			     BlendMode blendMode = BlendMode::BLENDMODE_ALPHA, D3D12_PRIMITIVE_TOPOLOGY_TYPE topologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE);
		~Pipeline();
		void Create(std::vector<ID3DBlob*> blobs, bool depthFlag, int rtvCount);

		ID3D12PipelineState* GetSolidPso();
		ID3D12PipelineState* GetWirePso();
	};
}