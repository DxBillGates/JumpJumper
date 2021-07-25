#include "..\..\Header\Collision\CollisionTreeCell.h"
#include "..\..\Header\Collision\CollisionTreeObject.h"

GatesEngine::CollisionTreeCell::CollisionTreeCell()
	: latestTreeObj(nullptr)
{
}

GatesEngine::CollisionTreeCell::~CollisionTreeCell()
{
}

bool GatesEngine::CollisionTreeCell::PushObject(CollisionTreeObject* pushObj)
{
	if (!pushObj)return false;
	if (pushObj->GetSettingCell() == this)return false;

	//pushObj->Remove();
	if (!latestTreeObj)latestTreeObj = pushObj;
	else
	{
		if (pushObj == latestTreeObj)
		{
			return false;
		}

		pushObj->SetNextObject(latestTreeObj);
		latestTreeObj->SetObject(pushObj);

		latestTreeObj = pushObj;
	}

	pushObj->RegisterCell(this);

	return true;
}

bool GatesEngine::CollisionTreeCell::OnRemove(CollisionTreeObject* removeobj)
{
	if (latestTreeObj == removeobj)
	{
		if (latestTreeObj)
		{
			latestTreeObj = latestTreeObj->GetNextObject();
		}
		else
		{
			return false;
		}
	}
	else
	{
		return false;
	}
	return true;;
}

GatesEngine::CollisionTreeObject* GatesEngine::CollisionTreeCell::GetFirstObject()
{
	return latestTreeObj;
}
