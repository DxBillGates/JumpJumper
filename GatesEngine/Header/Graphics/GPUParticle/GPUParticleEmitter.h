#pragma once
#include "GPUParticleManager.h"
#include "..\ComputePipeline.h"
#include "..\Camera.h"
#include "..\..\Graphics\Texture.h"

namespace GatesEngine
{
	struct EmitterData
	{
		// エミッターの状態
		int state;

		// エミッターのローカル座標
		Math::Vector3 pos;

		// エミッターから放出されるときのベクトル
		Math::Vector3 startForceMin, startForceMax;
		Math::Vector3 startForce;

		// エミッターが放っている力
		Math::Vector3 vel;

		// 追加データ
		float MAX_LIFE;
		Math::Vector3 initScale;
		float initAlpha;
	};

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
		EmitterData* addData;
		//int usingParticeCount;
		//int newUseParticleCount;
		float cycle;
	public:
		GPUParticleEmitter();
		virtual ~GPUParticleEmitter();
		virtual void Update(int addParticleValue = 1, float addCyclePerFrame = 1);
		virtual void Draw(Camera* camera, ComputePipeline* computeShader, const Math::Vector3& pos, Texture* tex = nullptr);
		void Create(GPUParticleManager* manager, UINT useParticleValue);
		EmitterData* GetAddData();
	};
}
