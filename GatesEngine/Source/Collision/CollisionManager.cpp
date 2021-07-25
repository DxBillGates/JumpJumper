#include "..\..\Header\Collision\CollisionManager.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"
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
		printf("コリジョンツリーに設定できませんでした、強制終了します\n");
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
		printf("判定回数:new->%d,old->%d  コライダーの数:%d  本来の総当り数:%d\n", count / 2, old / 2,(int)colliderComponents.size(),(int)pow((int)colliderComponents.size(),2));
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
				colliderList[i+1]->GetGameObject()->Collision(colliderList[(size_t)i]->GetGameObject());
			}
		}
		if (colliderList[i]->GetType() == colliderList[(size_t)i + 1]->GetType() && colliderList[i]->GetType() == sphere)
		{
			if (CheckSphereToSphere(colliderList[i], colliderList[(size_t)i + 1]));
		}
	}
}

bool GatesEngine::CollisionManager::CheckAABB(Collider* collider1, Collider* collider2)
{
	Math::Vector3 min1, min2, max1, max2;
	Transform* mTransform = collider1->GetGameObject()->GetTransform();
	Transform* oTransform = collider2->GetGameObject()->GetTransform();
	Math::Vector3 mColliderSize, oColliderSize;
	mColliderSize = collider1->GetSize();
	oColliderSize = collider2->GetSize();
	min1 = mTransform->position - mColliderSize / 2;
	max1 = mTransform->position + mColliderSize / 2;
	min2 = oTransform->position - oColliderSize / 2;
	max2 = oTransform->position + oColliderSize / 2;

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
	return false;
}

void GatesEngine::CollisionManager::Initialize(int level, const Math::Vector3& min, const Math::Vector3& max)
{
	collisionTreeManager = new CollisionTreeManager();
	collisionTreeManager->Initialize(level, min, max);
}
