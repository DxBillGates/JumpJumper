#pragma once
#include "RootSignature.h"
#include <d3d12.h>
#include <string>
#include <vector>

namespace GatesEngine
{
	/// <summary>
	/// �R���s���[�g�V�F�[�_��������悤�ɂ���֐�
	/// </summary>
	class ComputePipeline
	{
	private:
		GraphicsDevice* graphicsDevice;
		ID3DBlob* shaderBlob;
		ID3D12PipelineState* pipeline;
		RootSignature* rootSignature;
	public:
		/// <summary>
		/// �R���X�g���N�^�A���V�F�[�_�[�R���p�C��
		/// </summary>
		/// <param name="pGraphicsDevice">GraphicsDevice�̃|�C���^</param>
		/// <param name="filename">�R���p�C������V�F�[�_�[�t�@�C���̖��O</param>
		ComputePipeline(GraphicsDevice* pGraphicsDevice, const std::wstring& filename);
		~ComputePipeline();

		/// <summary>
		/// �����֐�
		/// </summary>
		/// <param name="range">�V�F�[�_�[�ň���RootParam�̔z��</param>
		void Create(const std::vector<RangeType>& range);

		/// <summary>
		/// �V�F�[�_�[���Z�b�g����֐�
		/// </summary>
		void Set();
	};
}
