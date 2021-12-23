#include "EnemyManager.h"
#include "Header/Input/Input.h"

void EnemyManager::ResetAllGroup()
{
	// 全グループを未使用状態＆敵を未登録状態に変更
	for (auto& group : enemyGroup)
	{
		group.isUse = false;
		for (auto& e : group.enemies)
		{
			e = nullptr;
		}
	}
}

void EnemyManager::ResetGroup(EnemyGroup* resetGroup)
{
	if (!resetGroup)return;

	// 指定グループを初期化
	resetGroup->isUse = false;
	for (auto& e : resetGroup->enemies)
	{
		if (!e)break;
		e->enemyClass->SetIsUse(false);
		e = nullptr;
	}

}

int EnemyManager::CheckUsingGroupCount()
{
	// 全グループを走査し使用しているグループの数を確認
	usingGroupCount = 0;
	for (auto& group : enemyGroup)
	{
		if (group.isUse)++usingGroupCount;
	}
	return usingGroupCount;
}

void EnemyManager::CheckAllGroup()
{
	for (auto& group : enemyGroup)
	{
		int liveCount = 0;
		if (!group.isUse)continue;
		for (auto& enemy : group.enemies)
		{
			if (!enemy)continue;
			if (enemy->gameObject->GetEnabled())++liveCount;
		}
		if (liveCount == 0)
		{
			ResetGroup(&group);
		}
	}
}

EnemyGroup* EnemyManager::CreatGroup(int useEnemyCount)
{
	// グループにする敵の数が最大値を超えるてるかチェック
	if (useEnemyCount > MAX_USE_ENEMY_PER_GROUP)return nullptr;

	EnemyGroup* useGroup = nullptr;
	for (auto& group : enemyGroup)
	{
		// グループが使用されているなら次のグループを確認
		if (group.isUse)continue;

		useGroup = &group;
		break;
	}

	// 使えるグループがあったかチェック
	if (!useGroup)return nullptr;

	// 登録した数
	int registerNumber = 0;

	// 敵リストを走査して使われていない敵を取得
	for (auto& enemy : enemies)
	{
		if (registerNumber >= useEnemyCount)break;
		if (!enemy.enemyClass)continue;
		if (enemy.enemyClass->GetIsUse())continue;

		// グループに敵を登録してその敵を初期化
		(*useGroup).enemies[registerNumber] = &enemy;
		enemy.enemyClass->Initialize();
		enemy.gameObject->Start();
		enemy.enemyClass->SetIsUse(true);
		(*useGroup).isUse = true;

		++registerNumber;
	}

	return &(*useGroup);
}

void EnemyManager::CreateStartBattleScene(int currentBattleNumber)
{
	const int MULTIPLE_GROUP_COUNT = 1;
	int createGroupCount = currentBattleNumber + 1;

	const int USE_ENEMY_COUNT = 5;

	EnemyGroup* createdGroup = nullptr;
	// バトル番号+1*定数 /個のグループを作成
	for (int i = 0; i < createGroupCount * MULTIPLE_GROUP_COUNT; ++i)
	{
		createdGroup = CreatGroup(USE_ENEMY_COUNT);
		if (!createdGroup)continue;

		for (auto& enemy : (*createdGroup).enemies)
		{
			if (!enemy)break;
			//enemy->enemyClass->SetPosition({});
		}
	}
}

EnemyManager::EnemyManager()
	: MAX_REGISTER_ENEMY(100)
	, registerEnemyCount(0)
	, MAX_USE_ENEMY_PER_GROUP(10)
	, MAX_ENEMY_GROUP(20)
	, enemyGroup()
	, usingGroupCount(0)
	, preUsingGroupCount(0)
	, battleSceneNumber(-1)
{
	// 敵リストを初期化
	enemies.resize(MAX_REGISTER_ENEMY);
	// 敵グループを最大数空作成
	enemyGroup.resize(MAX_ENEMY_GROUP);

	for (auto& group : enemyGroup)
	{
		group.enemies.resize(MAX_USE_ENEMY_PER_GROUP);
	}

	ResetAllGroup();
}

EnemyManager::~EnemyManager()
{
	ResetAllGroup();
	for (int i = 0; i < registerEnemyCount; ++i)
	{
		enemies[i].enemyClass->Initialize();
		enemies[i].gameObject->SetEnabled(false);
	}
}

void EnemyManager::Initialize()
{
	battleSceneNumber = -1;
}

void EnemyManager::Update(int currentBattleNumber)
{
	CreateGroupState createGroupState = CreateGroupState::NONE;
	if (currentBattleNumber != battleSceneNumber)createGroupState = CreateGroupState::START_BATTLE_SCENE;

	preUsingGroupCount = usingGroupCount;
	// すべてのグループをチェックして使われているか、グループが生存しているかを確認し更新
	CheckAllGroup();
	// 使っているグループ数を更新
	CheckUsingGroupCount();

	//if (preUsingCount > 0 && usingGroupCount == 0)
	if (GatesEngine::Input::GetInstance()->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::L))
	{
		createGroupState = CreateGroupState::START_BATTLE_SCENE;
	}


	switch (createGroupState)
	{
	case CreateGroupState::NONE:
		break;
	case CreateGroupState::START_BATTLE_SCENE:
		CreateStartBattleScene(currentBattleNumber);
		break;
	case CreateGroupState::BATTLE_SCENE:
		break;
	default:
		break;
	}

	// 現在のバトルシーンの番号を更新
	battleSceneNumber = currentBattleNumber;
}

Enemy* EnemyManager::RegisterEnemy(Enemy* addEnemy, GatesEngine::GameObject* gameObject)
{
	// 敵配列に登録
	enemies[registerEnemyCount].enemyClass = addEnemy;
	enemies[registerEnemyCount].gameObject = gameObject;

	// 追加登録した敵の数を増やす
	++registerEnemyCount;
	return addEnemy;
}

bool EnemyManager::IsDestroyAllGroup()
{
	bool result = preUsingGroupCount > 0 && usingGroupCount == 0 ? true : false;
	return result;
}
