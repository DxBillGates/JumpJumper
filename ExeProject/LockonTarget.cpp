#include "LockonTarget.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Manager/ShaderManager.h"
#include "Header/Graphics/Manager/MeshManager.h"
#include "Header/Graphics/Graphics.h"

LockonTarget::LockonTarget()
	: target(nullptr)
	, lockonTime(0)
	, MAX_LOCKON_TIME(5)
	, isLockon(false)
{
}

LockonTarget::~LockonTarget()
{
}

void LockonTarget::Initialize()
{
	BaseDataInitialize();
	AnimationDataInitialize();
}

void LockonTarget::BaseDataInitialize()
{
	target = nullptr;
	lockonTime = 0;
	isLockon = false;
}

void LockonTarget::AnimationDataInitialize()
{
	scaleAnimationTime = 0;
	angle = 0;
}

void LockonTarget::Update()
{
	if (!target)return;
	if (!target->GetEnabled())Initialize();
	if (lockonTime >= MAX_LOCKON_TIME)Initialize();

	const float PER_FRAME = 1.0f / 60.0f;
	lockonTime += PER_FRAME;


	if (scaleAnimationTime >= 1)
	{
		scaleAnimationTime = 1;
	}
	const float MAX_SCALE_ANIMATION_TIME = 0.5f;
	scaleAnimationTime += PER_FRAME / MAX_SCALE_ANIMATION_TIME;
	angle += PER_FRAME;
}

void LockonTarget::Draw(GatesEngine::GraphicsDevice* gDevice,const GatesEngine::Math::Matrix4x4& billBoardData)
{
	float addDrawSize = GatesEngine::Math::Easing::EaseOutExpo(scaleAnimationTime);
	GatesEngine::Math::Quaternion rotate = GatesEngine::Math::Quaternion({ {0,0,1},angle });

	float d = 1;
	if (d < 1)d = 1;
	float addScale = (MAX_LOCKON_TIME - lockonTime) / MAX_LOCKON_TIME;
	d *= GatesEngine::Math::Easing::EaseOutQuad(addScale) + addDrawSize;

	//GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultMeshShader")->Set();
	//gDevice->GetCBVSRVUAVHeap()->Set();
	//gDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 500 * d }) * GatesEngine::Math::Quaternion::Rotation(rotate) * billBoardData * GatesEngine::Math::Matrix4x4::Translate(target->GetTransform()->position));
	//gDevice->GetMainCamera()->Set(2);
	//gDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,0,1),GatesEngine::Math::Vector4(0,0,0,1) });
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("Texture")->Set();
	gDevice->GetCBVSRVUAVHeap()->Set();
	gDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 500 * d }) * GatesEngine::Math::Quaternion::Rotation(rotate) * billBoardData * GatesEngine::Math::Matrix4x4::Translate(target->GetTransform()->position));
	gDevice->GetMainCamera()->Set(2);
	GatesEngine::ResourceManager::GetTextureManager()->GetTexture("ui_lockon")->Set(5);
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();
}

void LockonTarget::SetLockonTarget(GatesEngine::GameObject* target)
{
	if (!target)return;
	this->target = target;
	isLockon = true;
}

GatesEngine::GameObject* LockonTarget::GetTarget()
{
	return target;
}

float LockonTarget::GetLockTime()
{
	return lockonTime;
}

bool LockonTarget::GetIsLockon()
{
	return isLockon;
}

const int LockonTarget::GetMaxLockonTime()
{
	return MAX_LOCKON_TIME;
}
