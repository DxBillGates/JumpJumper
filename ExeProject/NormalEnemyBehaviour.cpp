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
	//, deadPos({})
	//, isBossAttack(false)
	//, isAnimetion(false)
	//, animationTime(0)
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
	//deadPos = {};
	//isBossAttack = false;
	//isAnimetion = false;
	//animationTime = 0;
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

	for (auto& b : bullets)
	{
		b->SetPos({ 0,-1000,0 });
	}
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


		bool isMove = false;
		isMove = true;

		if (isMove)
		{
			gameObject->GetTransform()->position.y = gameObject->GetTransform()->position.y + sinf(t) * 10;
			t += 0.016f * 5;
		}

		gameObject->GetTransform()->position += moveVector;

		Shot();

		if (isTargetInCollder)
		{
			isTargetInCollder = false;
		}
	}

	FixBulletPos();

	Enemy::SetPosition(gameObject->GetTransform()->position);
}

void NormalEnemyBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();
	//GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set(false);

	GatesEngine::Transform* transform = gameObject->GetTransform();
	GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(transform->scale);
	GatesEngine::Math::Matrix4x4 rotateMatrix = GatesEngine::Math::Matrix4x4::Identity();
	GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(transform->position);

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
			isTargetInCollder = true;
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

void NormalEnemyBehaviour::AddBullet(Bullet* addBullet)
{
	bullets.push_back(addBullet);
}

void NormalEnemyBehaviour::Shot()
{
	const int INTERVAL = 1;
	if (shotInterval > INTERVAL)
	{
		shotInterval = 0;
	}
	else
	{
		shotInterval += 0.016f / 2.0f;
		return;
	}


	bool isShot = false;
	for (auto& b : bullets)
	{
		if (b->IsUse())continue;

		if (!isShot)
		{
			if (!target)continue;
			b->SetTarget(target, 0.1f, GatesEngine::Math::Vector3(0, 1, 0), 5000);
			isShot = true;
		}
		else
		{
			b->SetPos(gameObject->GetTransform()->position);
		}
	}
}

void NormalEnemyBehaviour::FixBulletPos()
{
	// Žg‚Á‚Ä‚¢‚È‚¢’e‚ÌˆÊ’u‚ð•â³
	for (auto& b : bullets)
	{
		if (b->IsUse())continue;
		b->SetPos(gameObject->GetTransform()->position);
	}
}
