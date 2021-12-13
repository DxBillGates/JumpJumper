#include "NormalEnemyBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "Header/Graphics/CBufferStruct.h"
#include "Header/Component/Collider.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Util/Random.h"
#include "PlayerBulletBehaviour.h"

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
	isEnabled = true;
	hp = 10;
	t = 0;
	deadPos = {};
	isBossAttack = false;
	isAnimetion = false;
	animationTime = 0;
	//hp = 10;
	Enemy::Initialize();

	//Enemy::pos = gameObject->GetTransform()->position;
	GatesEngine::Math::Vector3 p;
	p.x = GatesEngine::Random::Rand(-10000, 10000);
	p.y = GatesEngine::Random::Rand(100, 10000);
	p.z = GatesEngine::Random::Rand(-10000, 10000);
	Enemy::iPos = p;
	Enemy::SetTime(1);
	Enemy::SetTarget(gameObject->GetTransform()->position, {});

	shotInterval = 0;
}

void NormalEnemyBehaviour::Update()
{
	//Enemy::vel = {1,0,0};
	Enemy::Update();
	GatesEngine::Math::Vector3 fixPos = Enemy::pos;
	gameObject->GetTransform()->position = fixPos;

	if (!Enemy::GetIsTarget())
	{
		GatesEngine::Math::Vector3 moveVector;
		if (hp <= 0 && !isBossAttack && !isAnimetion)
		{
			isAnimetion = true;
			deadPos = gameObject->GetTransform()->position;
		}

		const float ANIMATION_TIME = 2;
		if (isAnimetion)
		{
			bool isBreak = (animationTime >= ANIMATION_TIME) ? true : false;

			if (!isBreak)
			{
				gameObject->GetTransform()->position = deadPos;
				//moveVector = boss->GetTransform()->position - gameObject->GetTransform()->position;
				//moveVector = moveVector.Normalize();
				float range = 32767;
				GatesEngine::Math::Vector3 randomVector = { GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range),GatesEngine::Random::Rand(-range,range) };
				moveVector = randomVector;
				moveVector = moveVector.Normalize() * 20;
			}
			else
			{
				isAnimetion = false;
				isBossAttack = true;
				animationTime = 0;
			}
			animationTime += 0.016f;
		}

		if (isBossAttack)
		{
			moveVector = boss->GetTransform()->position - gameObject->GetTransform()->position;
			moveVector = moveVector.Normalize() * 10 * animationTime;
			animationTime += 0.016f;
		}

		bool isMove = false;
		//if (target)
		//{
		//	moveVector = target->GetTransform()->position - gameObject->GetTransform()->position;
		//	moveVector = moveVector.Normalize();

		//}
		isMove = true;

		if (isMove)
		{
			gameObject->GetTransform()->position.y = gameObject->GetTransform()->position.y + sinf(t) * 10;
			t += 0.016f * 5;
		}

		//gameObject->GetTransform()->scale = gameObject->GetTransform()->scale + moveVector.Normalize();
		gameObject->GetTransform()->position += moveVector;

		//target = nullptr;

		const int INTERVAL = 1;
		if (shotInterval > INTERVAL)
		{
			Shot();
			shotInterval = 0;
		}

		shotInterval += 0.016f / 2.0f;
	}

	for (auto& b : bullets)
	{
		if (b->IsUse())continue;
		b->SetPos(gameObject->GetTransform()->position);
	}
	//Enemy::SetPosition(fixPos);

	Enemy::SetPosition(gameObject->GetTransform()->position);
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);

	GatesEngine::Transform* transform = gameObject->GetTransform();
	GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(transform->scale);
	GatesEngine::Math::Matrix4x4 rotateMatrix = GatesEngine::Math::Matrix4x4::Identity();
	GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(transform->position);

	static float a = 0;
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::G))
	{
		a += 1;
	}
	if (target)
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * rotateMatrix * posMatrix);
	}
	else
	{
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	}
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,0.5f,0,spawnTime} });
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
		Enemy::Initialize();
		Start();
		gameObject->Start();
		gameObject->SetEnabled(false);
	}

	if (other->GetTag() == "Boss")
	{
		Enemy::Initialize();
		Start();
		gameObject->Start();
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

void NormalEnemyBehaviour::SetTarget(GatesEngine::GameObject* t)
{
	target = t;
}

void NormalEnemyBehaviour::AddBullet(PlayerBullet* addBullet)
{
	bullets.push_back(addBullet);
}

void NormalEnemyBehaviour::Shot()
{
	bool isShot = false;
	for (auto& b : bullets)
	{
		if (b->IsUse())continue;

		if (!isShot)
		{
			if (!target)continue;
			b->SetTarget(target, 0.1f, GatesEngine::Math::Vector3(0,1,0),5000);
			isShot = true;
		}
		else
		{
			b->SetPos(gameObject->GetTransform()->position);
		}
	}
}
