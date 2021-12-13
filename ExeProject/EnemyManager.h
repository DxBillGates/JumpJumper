#pragma once
#include "Enemy.h"
#include "Header/GameObject/GameObject.h"
#include <vector>

struct EnemyInfo
{
	Enemy* enemyClass;
	GatesEngine::GameObject* gameObject;
};
struct EnemyGroup
{
	bool isUse;
	int enemyCount;
	std::vector<EnemyInfo*> enemies;

	EnemyGroup()
		: isUse(false)
		, enemyCount(0)
	{}
};

enum class CreateGroupState
{
	NONE,
	START_BATTLE_SCENE,
	BATTLE_SCENE,
};

// 通常敵戦で扱う敵管理クラス、隊列を組ませたり行動を決定させる
class EnemyManager
{
private:
	const int MAX_REGISTER_ENEMY;
	int registerEnemyCount;
	std::vector<EnemyInfo> enemies;

	const int MAX_ENEMY_GROUP;
	// ひとつのグループで使用できる敵の最大数
	const int MAX_USE_ENEMY_PER_GROUP;
	std::vector<EnemyGroup> enemyGroup;

	// 現在のバトルシーンで敵グループをどれだけ使用しているか確認するための変数
	int usingGroupCount;
	// 前フレームでの使用敵グループ数
	int preUsingGroupCount;

	// 現在のバトルシーンでどれだけグループを使用したか確認するための関数
	int usingCurrentBattleSceneGroupCoutn;

	int battleSceneNumber;
private:
	void ResetAllGroup();
	void ResetGroup(EnemyGroup* resetGroup);
	int CheckUsingGroupCount();
	void CheckAllGroup();
	EnemyGroup* CreatGroup(int useEnemyCount);
	void CreateStartBattleScene(int currentBattleNumber);
public:
	EnemyManager();
	~EnemyManager();
	void Initialize();
	void Update(int currentBattleNumber);
	Enemy* RegisterEnemy(Enemy* addEnemy,GatesEngine::GameObject* gameObject);
	bool IsDestroyAllGroup();
};

