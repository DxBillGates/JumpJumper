#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"

NormalEnemyBehaviour::NormalEnemyBehaviour()
	: target(nullptr)
	, boss(nullptr)
	, hp(10)
	, t(0)
	, deadPos({})
	, isBossAttack(false)
	, isAnimetion(false)
	, animationTime(0)
{
}

NormalEnemyBehaviour::~NormalEnemyBehaviour()
{
}

void NormalEnemyBehaviour::Start()
{
	hp = 10;
	//Enemy::Initialize();
	Enemy::pos = gameObject->GetTransform()->position;
}

void NormalEnemyBehaviour::Update()
{
	//Enemy::vel = {1,0,0};
	Enemy::Update();
	GatesEngine::Math::Vector3 fixPos = Enemy::pos;
	gameObject->GetTransform()->position = fixPos;

	if (!Enemy::GetIsTarget())
	{

	}

	Enemy::SetPosition(gameObject->GetTransform()->position);
	//Enemy::SetPosition(fixPos);

	//GatesEngine::Math::Vector3 moveVector;
	//if (hp <= 0 && !isBossAttack && !isAnimetion)
	//{
	//	isAnimetion = true;
	//	deadPos = gameObject->GetTransform()->position;
	//}

	//const float ANIMATION_TIME = 2;
	//if (isAnimetion)
	//{
	//	bool isBreak = (animationTime >= ANIMATION_TIME) ? true : false;

	//	if (!isBreak)
	//	{
	//		gameObject->GetTransform()->position = deadPos;
	//		//moveVector = boss->GetTransform()->position - gameObject->GetTransform()->position;
	//		//moveVector = moveVector.Normalize();
	//		float range = 32767;
	//		GatesEngine::Math::Vector3 randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) };
	//		moveVector = randomVector;
	//		moveVector = moveVector.Normalize() * 20;
	//	}
	//	else
	//	{
	//		isAnimetion = false;
	//		isBossAttack = true;
	//		animationTime = 0;
	//	}
	//	animationTime += 0.016f;
	//}

	//if (isBossAttack)
	//{
	//	moveVector = boss->GetTransform()->position - gameObject->GetTransform()->position;
	//	moveVector = moveVector.Normalize() * 10 * animationTime;
	//	animationTime += 0.016f;
	//}

	//bool isMove = false;
	////if (target)
	////{
	////	moveVector = target->GetTransform()->position - gameObject->GetTransform()->position;
	////	moveVector = moveVector.Normalize();

	////}
	//isMove = true;

	//if (isMove)
	//{
	//	gameObject->GetTransform()->position.y = gameObject->GetTransform()->position.y + sinf(t) * 10;
	//	t += 0.016f;
	//}

	////gameObject->GetTransform()->scale = gameObject->GetTransform()->scale + moveVector.Normalize();
	//gameObject->GetTransform()->position += moveVector;

	//target = nullptr;
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("testModel")->Draw();
}

void NormalEnemyBehaviour::OnCollision(GatesEngine::Collider* otherCollider)
{
	GatesEngine::GameObject* other = otherCollider->GetGameObject();
	if (!target)
	{
		if (other->GetName() == "player")
		{
			target = other;
		}
	}

	if (other->GetTag() == "playerBullet")
	{
		hp = 0;
	}

	if (other->GetTag() == "Boss")
	{
		Enemy::Initialize();
		gameObject->SetEnabled(false);
	}

}

void NormalEnemyBehaviour::Damage(float value)
{
	hp -= value;
}

float NormalEnemyBehaviour::GetHP()
{
	return hp;
}

void NormalEnemyBehaviour::SetBoss(GatesEngine::GameObject* gameObject)
{
	boss = gameObject;
}
