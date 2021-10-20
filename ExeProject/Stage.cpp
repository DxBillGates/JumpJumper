#include "Stage.h"

Stage::Stage()
    : needKillCount(0)
    , gCollisionManager({})
{
}

Stage::~Stage()
{
}

int Stage::GetNeedKillCount()
{
    return needKillCount;
}

void Stage::Update()
{
    gCollisionManager.Update();
}

GameObjectCollisionManager* Stage::GetCollisionManager()
{
    return &gCollisionManager;
}
