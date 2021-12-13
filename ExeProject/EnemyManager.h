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

// �ʏ�G��ň����G�Ǘ��N���X�A�����g�܂�����s�������肳����
class EnemyManager
{
private:
	const int MAX_REGISTER_ENEMY;
	int registerEnemyCount;
	std::vector<EnemyInfo> enemies;

	const int MAX_ENEMY_GROUP;
	// �ЂƂ̃O���[�v�Ŏg�p�ł���G�̍ő吔
	const int MAX_USE_ENEMY_PER_GROUP;
	std::vector<EnemyGroup> enemyGroup;

	// ���݂̃o�g���V�[���œG�O���[�v���ǂꂾ���g�p���Ă��邩�m�F���邽�߂̕ϐ�
	int usingGroupCount;
	// �O�t���[���ł̎g�p�G�O���[�v��
	int preUsingGroupCount;

	// ���݂̃o�g���V�[���łǂꂾ���O���[�v���g�p�������m�F���邽�߂̊֐�
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

