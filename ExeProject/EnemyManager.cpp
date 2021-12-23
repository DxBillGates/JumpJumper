#include "EnemyManager.h"
#include "Header/Input/Input.h"

void EnemyManager::ResetAllGroup()
{
	// �S�O���[�v�𖢎g�p��ԁ��G�𖢓o�^��ԂɕύX
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

	// �w��O���[�v��������
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
	// �S�O���[�v�𑖍����g�p���Ă���O���[�v�̐����m�F
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
	// �O���[�v�ɂ���G�̐����ő�l�𒴂���Ă邩�`�F�b�N
	if (useEnemyCount > MAX_USE_ENEMY_PER_GROUP)return nullptr;

	EnemyGroup* useGroup = nullptr;
	for (auto& group : enemyGroup)
	{
		// �O���[�v���g�p����Ă���Ȃ玟�̃O���[�v���m�F
		if (group.isUse)continue;

		useGroup = &group;
		break;
	}

	// �g����O���[�v�����������`�F�b�N
	if (!useGroup)return nullptr;

	// �o�^������
	int registerNumber = 0;

	// �G���X�g�𑖍����Ďg���Ă��Ȃ��G���擾
	for (auto& enemy : enemies)
	{
		if (registerNumber >= useEnemyCount)break;
		if (!enemy.enemyClass)continue;
		if (enemy.enemyClass->GetIsUse())continue;

		// �O���[�v�ɓG��o�^���Ă��̓G��������
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
	// �o�g���ԍ�+1*�萔 /�̃O���[�v���쐬
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
	// �G���X�g��������
	enemies.resize(MAX_REGISTER_ENEMY);
	// �G�O���[�v���ő吔��쐬
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
	// ���ׂẴO���[�v���`�F�b�N���Ďg���Ă��邩�A�O���[�v���������Ă��邩���m�F���X�V
	CheckAllGroup();
	// �g���Ă���O���[�v�����X�V
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

	// ���݂̃o�g���V�[���̔ԍ����X�V
	battleSceneNumber = currentBattleNumber;
}

Enemy* EnemyManager::RegisterEnemy(Enemy* addEnemy, GatesEngine::GameObject* gameObject)
{
	// �G�z��ɓo�^
	enemies[registerEnemyCount].enemyClass = addEnemy;
	enemies[registerEnemyCount].gameObject = gameObject;

	// �ǉ��o�^�����G�̐��𑝂₷
	++registerEnemyCount;
	return addEnemy;
}

bool EnemyManager::IsDestroyAllGroup()
{
	bool result = preUsingGroupCount > 0 && usingGroupCount == 0 ? true : false;
	return result;
}
