#include "RotateAxisParticleManager.h"
#include "Header/GameObject/GameObject.h"

RotateAxisParticleManager::RotateAxisParticleManager()
{
}

RotateAxisParticleManager::~RotateAxisParticleManager()
{
}

void RotateAxisParticleManager::Initialize()
{
	for (auto& p : particleBehaviours)
	{
		p->SetAddParticleInfo(1, 0);
		p->GetGameObject()->SetEnabled(false);
	}
}

void RotateAxisParticleManager::Add(RotateAxisBehaviour* axis, DefaultGPUParticleEmitterBehaviour* particle)
{
	axisBehaviours.push_back(axis);
	particleBehaviours.push_back(particle);
}

void RotateAxisParticleManager::StartParticles(const GatesEngine::Math::Vector3& center)
{
	for (auto& a : axisBehaviours)
	{
		a->SetCenter(center);
		a->GetGameObject()->SetEnabled(true);
	}
}

void RotateAxisParticleManager::ReverseParticle()
{
	for (auto& a : axisBehaviours)
	{
		a->SetCenter(a->GetOriginPosition());
		//a->SetIsFaceCenter(false);
		a->SetAddValue(1);
		a->GetGameObject()->SetEnabled(true);
	}
}

bool RotateAxisParticleManager::GetIsEndParticles(const GatesEngine::Math::Vector3& center)
{
	bool flag = false;

	int particleEmitterCount = (int)particleBehaviours.size();
	int deadEmitterCount = 0;
	for (auto& p : particleBehaviours)
	{
		//flag = false;
		GatesEngine::Transform* t = p->GetGameObject()->GetTransform();
		float length = GatesEngine::Math::Vector3::Distance(center, t->position);

		const float MIN_LENGTH = 1000;
		if (length < MIN_LENGTH)
		{
			//flag = true;
			++deadEmitterCount;
		}
	}

	flag = (deadEmitterCount >= particleEmitterCount) ? true : false;
	return flag;
}

void RotateAxisParticleManager::EndParticles()
{
	for (auto& p : particleBehaviours)
	{
		p->SetAddParticleInfo(0, 0);
	}
}
