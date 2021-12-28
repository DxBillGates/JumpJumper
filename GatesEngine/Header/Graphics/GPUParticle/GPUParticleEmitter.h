#pragma once
#include "GPUParticleManager.h"
#include "..\ComputePipeline.h"
#include "..\Camera.h"

namespace GatesEngine
{
	class GPUParticleEmitter
	{
	protected:
		GPUParticleManager* manager;
		ParticleData* particleData;
		ParticleData* updateParticleData;
		UINT useParticleOffset;
		UINT useParticleValue;
		UINT uavValue;
		UINT srvValue;
		UINT addDataSrvValue;
		ID3D12Resource* addBuffer;
		ParticleData* addData;
		//int usingParticeCount;
		//int newUseParticleCount;
	public:
		GPUParticleEmitter();
		virtual ~GPUParticleEmitter();
		virtual void Update();
		virtual void Draw(Camera* camera, ComputePipeline* computeShader, const Math::Vector3& pos, const Math::Vector3& addVel = {});
		void Create(GPUParticleManager* manager,UINT useParticleValue);
	};
}
