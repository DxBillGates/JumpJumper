#include "..\..\Header\Collision\CollisionManager.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"
#include "..\..\Header\Collision\CollisionTreeManager.h"
#include "..\..\Header\GameObject\GameObject.h"
#include <assert.h>

GatesEngine::CollisionManager::CollisionManager()
	: collisionTreeManager(nullptr)
{
}

GatesEngine::CollisionManager::~CollisionManager()
{
	delete collisionTreeManager;
}

GatesEngine::Collider* GatesEngine::CollisionManager::AddColliderComponent(Collider* collider)
{
	colliderComponents.push_back(collider);
	return collider;
}

void GatesEngine::CollisionManager::AddRegisterTree(Collider* collider)
{
	if (!collisionTreeManager)return;
	CollisionTreeObject* treeObj = new CollisionTreeObject(collider);
	bool result = collisionTreeManager->Register(collider, treeObj);

	if (!result)
	{
		printf("�R���W�����c���[�ɐݒ�ł��܂���ł����A�����I�����܂�\n");
		assert(false);
	}

	collider->SetCollisionTreeObject(treeObj);
}

void GatesEngine::CollisionManager::UpdateTree(Collider* collider, CollisionTreeObject* object)
{
	if (!object)
	{
		AddRegisterTree(collider);
		return;
	}
	object->Remove();
	bool result = collisionTreeManager->Register(object->GetCollider(), object);
}

void GatesEngine::CollisionManager::Update()
{
	for (int i = 0; i < (int)colliderComponents.size(); ++i)
	{
		UpdateTree(colliderComponents[i], colliderComponents[i]->GetCollisionTreeObject());
	}

	CollisionCheck();
}

void GatesEngine::CollisionManager::CollisionCheck()
{
	std::vector<Collider*> colliderList;
	static int old = 0;
	int count = collisionTreeManager->GetAllCollisionListSize(colliderList);
	if (count != old)
	{
		printf("�����:new->%d,old->%d  �R���C�_�[�̐�:%d  �{���̑����萔:%d\n", count / 2, old / 2,(int)colliderComponents.size(),(int)pow((int)colliderComponents.size(),2));
	}
	old = count;

	if (count <= 0)
	{
		return;
	}
	for (int i = 0; i < count - 1; i += 2)
	{						
		if (!colliderList[i]->GetEnabled() || !colliderList[(size_t)i + 1]->GetEnabled())continue;
		ColliderType cube, sphere;
		cube = ColliderType::CUBE;
		sphere = ColliderType::SPHERE;
		if (colliderList[i]->GetType() ==  colliderList[(size_t)i + 1]->GetType() && colliderList[i]->GetType() == cube)
		{
			if (CheckAABB(colliderList[i], colliderList[(size_t)i + 1]))
			{
				colliderList[i]->GetGameObject()->Collision(colliderList[(size_t)i + 1]->GetGameObject());
				colliderList[i]->GetGameObject()->Collision(colliderList[(size_t)i + 1]);

				colliderList[(size_t)i+1]->GetGameObject()->Collision(colliderList[(size_t)i]->GetGameObject());
				colliderList[(size_t)i + 1]->GetGameObject()->Collision(colliderList[(size_t)i]);

				colliderList[i]->GetGameObject()->GetCollider()->SetColor({1,0,0,0});
				colliderList[(size_t)i+1]->GetGameObject()->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}
		if (colliderList[i]->GetType() == colliderList[(size_t)i + 1]->GetType() && colliderList[i]->GetType() == sphere)
		{
			if (CheckSphereToSphere(colliderList[i], colliderList[(size_t)i + 1]))
			{
				colliderList[i]->GetGameObject()->Collision(colliderList[(size_t)i + 1]->GetGameObject());
				colliderList[(size_t)i + 1]->GetGameObject()->Collision(colliderList[(size_t)i]->GetGameObject());

				colliderList[i]->GetGameObject()->GetCollider()->SetColor({ 1,0,0,0 });
				colliderList[(size_t)i + 1]->GetGameObject()->GetCollider()->SetColor({ 1,0,0,0 });
			}
		}
	}
}

bool GatesEngine::CollisionManager::CheckAABB(Collider* collider1, Collider* collider2)
{
	Math::Vector3 min1, min2, max1, max2;
	Transform* mTransform = collider1->GetGameObject()->GetTransform();
	Transform* oTransform = collider2->GetGameObject()->GetTransform();
	Transform* mcTransform = collider1->GetTransform();
	Transform* ocTransform = collider2->GetTransform();
	Math::Vector3 mColliderSize, oColliderSize;
	mColliderSize = { mTransform->scale.x * mcTransform->scale.x,mTransform->scale.y * mcTransform->scale.y,mTransform->scale.z * mcTransform->scale.z };
	oColliderSize = { oTransform->scale.x * ocTransform->scale.x,oTransform->scale.y * ocTransform->scale.y,oTransform->scale.z * ocTransform->scale.z };
	min1 = mTransform->position + mcTransform->position - mColliderSize / 2;
	max1 = mTransform->position + mcTransform->position + mColliderSize / 2;
	min2 = oTransform->position + ocTransform->position - oColliderSize / 2;
	max2 = oTransform->position + ocTransform->position + oColliderSize / 2;

	if (min1.x > max2.x)return false;
	if (max1.x < min2.x)return false;
	if (min1.y > max2.y)return false;
	if (max1.y < min2.y)return false;
	if (min1.z > max2.z)return false;
	if (max1.z < min2.z)return false;
	return true;
}

bool GatesEngine::CollisionManager::CheckSphereToSphere(Collider* collider1, Collider* collider2)
{
	Transform* mTransform;
	Transform* oTransform;
	mTransform = collider1->GetGameObject()->GetTransform();
	oTransform = collider2->GetGameObject()->GetTransform();

	Math::Vector3 mColliderSize, oColliderSize;
	mColliderSize = collider1->GetSize();
	oColliderSize = collider2->GetSize();

	float distance = GatesEngine::Math::Vector3::Distance(mTransform->position, oTransform->position);
	if (distance * distance <= powf(mColliderSize.x/2 + oColliderSize.x/2,2))return true;
	return false;
}

bool GatesEngine::CollisionManager::CheckAABBToRay(Collider* collider,const Math::Vector3& linePos, const Math::Vector3& lineDir)
{
	Math::Vector3 min1, min2, max1, max2;
	Transform* mTransform = collider->GetGameObject()->GetTransform();
	Transform* mcTransform = collider->GetTransform();
	Math::Vector3 mColliderSize, oColliderSize;
	mColliderSize = { mTransform->scale.x * mcTransform->scale.x,mTransform->scale.y * mcTransform->scale.y,mTransform->scale.z * mcTransform->scale.z };
	min1 = mTransform->position + mcTransform->position - mColliderSize / 2;
	max1 = mTransform->position + mcTransform->position + mColliderSize / 2;

	Math::Vector3 a = min1 - max1;

	float p[3], d[3], _min[3], _max[3];
	memcpy(p, &linePos, sizeof(Math::Vector3));
	memcpy(d, &lineDir, sizeof(Math::Vector3));
	memcpy(_min, &min1, sizeof(Math::Vector3));
	memcpy(_max, &max1, sizeof(Math::Vector3));

	float t = -FLT_MAX;
	float t_max = FLT_MAX;

	for (int i = 0; i < 3; ++i) 
	{
		if (abs(d[i]) < FLT_EPSILON) 
		{
			if (p[i] < _min[i] || p[i] > _max[i])
			{
				return false;
			}
		}
		else 
		{
			float odd = 1.0f / d[i];
			float t1 = (_min[i] - p[i]) * odd;
			float t2 = (_max[i] - p[i]) * odd;
			if (t1 > t2) {
				float tmp = t1; t1 = t2; t2 = tmp;
			}

			if (t1 > t) t = t1;
			if (t2 < t_max) t_max = t2;

			if (t >= t_max)
			{
				return false;
			}
		}
	}
	return true;
}

bool GatesEngine::CollisionManager::CheckAABBToSphere(Collider* collider1, Collider* collider2)
{
	Math::Vector3 min1, max1;
	Transform* mTransform = collider1->GetGameObject()->GetTransform();
	Transform* oTransform = collider2->GetGameObject()->GetTransform();
	Transform* mcTransform = collider1->GetTransform();
	Transform* ocTransform = collider2->GetTransform();
	Math::Vector3 sPos = ocTransform->position + oTransform->position;
	Math::Vector3 mColliderSize, oColliderSize;
	mColliderSize = { mTransform->scale.x * mcTransform->scale.x,mTransform->scale.y * mcTransform->scale.y,mTransform->scale.z * mcTransform->scale.z };
	oColliderSize = { oTransform->scale.x * ocTransform->scale.x,oTransform->scale.y * ocTransform->scale.y,oTransform->scale.z * ocTransform->scale.z };
	min1 = mTransform->position + mcTransform->position - mColliderSize / 2;
	max1 = mTransform->position + mcTransform->position + mColliderSize / 2;

	float minLength, maxLength;
	minLength = Math::Vector3::Distance(sPos, min1);
	maxLength = Math::Vector3::Distance(sPos, max1);

	float r = powf(oColliderSize.x / 2, 2);
	if (minLength * minLength <= r || maxLength * maxLength <= r)
	{
		return true;
	}

	return false;
}

void GatesEngine::CollisionManager::Initialize(int level, const Math::Vector3& min, const Math::Vector3& max)
{
	collisionTreeManager = new CollisionTreeManager();
	collisionTreeManager->Initialize(level, min, max);
}
