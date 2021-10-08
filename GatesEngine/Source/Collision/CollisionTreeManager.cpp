#include "..\..\Header\Collision\CollisionTreeManager.h"
#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"
#include <assert.h>

int GatesEngine::CollisionTreeManager::GetMortonNumber(const Math::Vector3& min, const Math::Vector3& max)
{
	int lt = GetPointElement(min);
	int rb = GetPointElement(max);

	int def = rb ^ lt;
	int highLevel = 1;
	for (int i = 0; i < minLevel; ++i)
	{
		int check = (def >> (i * 3)) & 0x7;
		if (check != 0)highLevel = i + 1;
	}

	int spaceNum = rb >> (highLevel * 3);
	int addNum = (pows[(size_t)minLevel - highLevel] - 1) / MAX_LEVEL;
	spaceNum += addNum;

	if (spaceNum > cellCount)return 0xffffffff;

	return spaceNum;
}

int GatesEngine::CollisionTreeManager::BitSeparete3D(int n)
{
	int s = n;
	s = (s | s << 8) & 0x0000f00f;
	s = (s | s << 4) & 0x000c30c3;
	s = (s | s << 2) & 0x00249249;
	return s;
}

int GatesEngine::CollisionTreeManager::Get3DMortonNumber(int x, int y, int z)
{
	return BitSeparete3D(x) | (BitSeparete3D(y) << 1) | (BitSeparete3D(z) << 2);
}

int GatesEngine::CollisionTreeManager::GetPointElement(const Math::Vector3& pos)
{
	Math::Vector3 xyz = (pos - minRange);
	xyz.x /= minAreaRange.x;
	xyz.y /= minAreaRange.y;
	xyz.z /= minAreaRange.z;
	return Get3DMortonNumber((int)xyz.x, (int)xyz.y, (int)xyz.z);
}

void GatesEngine::CollisionTreeManager::CreateNewCell(int registerArea)
{
	while (!cells[registerArea])
	{
		cells[registerArea] = new CollisionTreeCell();
		registerArea = (registerArea - 1) >> 3;

		if (registerArea >= cellCount)break;
		if (registerArea < 0)break;
	}
}

int GatesEngine::CollisionTreeManager::GetAllCollisionListSize(std::vector<Collider*>& colliders)
{
	colliders.clear();

	if (!cells[0])return 0;

	std::list<Collider*> colliderStack;
	GetCollisionList(0, colliders, colliderStack);

	return (int)colliders.size();
}

GatesEngine::CollisionTreeManager::CollisionTreeManager()
	: minRange(0)
	, maxRange(1)
	, range(1)
	, minAreaRange(1)
	, cellCount(0)
	, minLevel(0)
{
	pows.resize(MAX_LEVEL + 2);
}

GatesEngine::CollisionTreeManager::~CollisionTreeManager()
{
	for (auto itr = cells.begin(); itr != cells.end(); ++itr)
	{
		delete (*itr);
	}
	cells.clear();
}

void GatesEngine::CollisionTreeManager::Initialize(int level, const Math::Vector3& min, const Math::Vector3& max)
{
	if (level >= MAX_LEVEL)
	{
		printf("八分木空間分割の分割レベルがキャパオーバーです、最大値に設定して生成します\n");
		level = MAX_LEVEL - 1;
		//assert(false);
	}

	pows[0] = 1;
	for (int i = 1; i < MAX_LEVEL + 1; ++i)
	{
		//レベルごとの空間の数を算出
		pows[i] = pows[(size_t)i - 1] * MAX_LEVEL + 1;
	}

	//総空間数の算出とメモリ確保
	cellCount = (pows[(size_t)level + 1] - 1) / MAX_LEVEL;
	cells.resize(cellCount);

	minRange = min;
	maxRange = max;
	range = max - min;


	minAreaRange = range / ((float)(1 << level));

	minLevel = level;
}

bool GatesEngine::CollisionTreeManager::Register(Collider* registerCollider, CollisionTreeObject* obj)
{
	if (!registerCollider)return false;
	//if (registerCollider->GetType() != ColliderType::CUBE)return false;

	Math::Vector3 min, max;
	min = registerCollider->GetGameObject()->GetTransform()->position - registerCollider->GetSize() / 2;
	max = registerCollider->GetGameObject()->GetTransform()->position + registerCollider->GetSize() / 2;

	int registerArea = GetMortonNumber(min, max);
	//if (registerCollider->GetType() == ColliderType::SPHERE)registerArea = GetPointElement(registerCollider->GetGameObject()->GetTransform()->position);
	if (registerArea == -1)return false;
	if (registerArea < cellCount)
	{
		if (!cells[registerArea])CreateNewCell(registerArea);

		return cells[registerArea]->PushObject(obj);
	}

	return false;
}

bool GatesEngine::CollisionTreeManager::GetCollisionList(int elem, std::vector<Collider*>& colliders, std::list<Collider*>& colliderStacks)
{
	std::list<Collider*>::iterator it;
	CollisionTreeObject* treeObj = cells[elem]->GetFirstObject();

	while (treeObj)
	{
		CollisionTreeObject* treeObj2 = treeObj->GetNextObject();
		while (treeObj2)
		{
			//if (treeObj->GetCollider()->GetGameObject()->GetName() == treeObj2->GetCollider()->GetGameObject()->GetName())continue;
			//if (treeObj == treeObj2)
			//{
			//	printf("同じオブジェクト!!!!\n");
			//	//treeObj2 = treeObj2->GetNextObject();
			//	//continue;
			//	break;
			//}

			if (treeObj->GetCollider()->GetGameObject()->GetTag() == treeObj2->GetCollider()->GetGameObject()->GetTag())
			{
				treeObj2 = treeObj2->GetNextObject();
				continue;
			}
			colliders.push_back(treeObj->GetCollider());
			colliders.push_back(treeObj2->GetCollider());

			treeObj2 = treeObj2->GetNextObject();
		}

		for (it = colliderStacks.begin(); it != colliderStacks.end(); ++it)
		{
			colliders.push_back(treeObj->GetCollider());
			colliders.push_back(*it);
		}

		treeObj = treeObj->GetNextObject();
	}

	bool childFlag = false;
	int objectNum = 0;
	int nextElem = 0;

	for (int i = 0; i < 8; ++i)
	{
		nextElem = elem * 8 + 1 + i;
		if (nextElem < cellCount && cells[(size_t)elem * 8 + 1 + i])
		{
			if (!childFlag)
			{
				treeObj = cells[elem]->GetFirstObject();
				while (treeObj)
				{
					colliderStacks.push_back(treeObj->GetCollider());
					++objectNum;
					treeObj = treeObj->GetNextObject();
				}
			}
			childFlag = true;

			GetCollisionList(elem * 8 + 1 + i, colliders, colliderStacks);
		}
	}

	if (childFlag)
	{
		for (int i = 0; i < objectNum; ++i)
		{
			colliderStacks.pop_back();
		}
	}
	return true;
}
