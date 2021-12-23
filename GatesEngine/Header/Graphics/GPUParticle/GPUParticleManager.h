#pragma once
#include "..\..\Math\Math.h"
#include "..\GraphicsDevice.h"
#include "..\ComputePipeline.h"
#include <d3d12.h>

namespace GatesEngine
{
	struct ParticleData
	{
		// �ǂ̃G�~�b�^�[�ɑ����Ă��邩
		int emitter;
		// �[�x�l�i�\�[�g�p�j
		float depth;

		// ���
		int state;
		// �����t���O
		int isDead;
		// ���C�t
		float life;

		// ��b�f�[�^
		Math::Vector3 scale;
		Math::Vector3 rotate;
		Math::Vector4 pos;
		Math::Vector4 vel;
		float alpha;
	};

	class GPUParticleManager
	{
	private:
		const UINT maxParticleValue;
		UINT nextParticleOffset;
		UINT maxDescriptorValue;
		UINT uavNextOffset;
		UINT srvNextOffset;

		ID3D12Resource* particleBuffer;
		ID3D12Resource* updateParticleBuffer;
		ID3D12DescriptorHeap* heap;

		GraphicsDevice* graphicsDevice;
	public:
		GPUParticleManager(GraphicsDevice* pGraphicsDevice);
		~GPUParticleManager();
		UINT GetMaxParticleValue();
		UINT GetNextParticleOffset();
		UINT GetUavNextOffset();
		UINT GetSrvNextOffset();
		ID3D12Resource* GetParticleBuffer();
		ID3D12Resource* GetUpdateParticleBuffer();
		ID3D12DescriptorHeap* GetHeap();

		void IncrementParticleOffset(UINT value);
		void CreateUAV(const D3D12_UNORDERED_ACCESS_VIEW_DESC& viewDesc);
		void CreateSRV(ID3D12Resource* buffer,const D3D12_SHADER_RESOURCE_VIEW_DESC& viewDesc);

		GraphicsDevice* GetDevice();

		void Update();
		void Dispatch(ComputePipeline* shader);
	};
}