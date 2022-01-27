#pragma once
#include "Header/Component/Behaviour.h"
#include "LockonTarget.h"
#include "Header/Math/Math.h"
#include "Header/Input/Input.h"
#include "Header/Graphics/Camera.h"
#include "PlayerBullet.h"
#include "PlayerCamera.h"

#include <vector>
#include <string>
#include <tuple>
#include <algorithm>

class PlayerBehaviour : public GatesEngine::Behaviour
{
private:
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Math::Vector3 moveVel;
	GatesEngine::Math::Vector3 jetVel;
	GatesEngine::Math::Vector3 gVel;
	bool isJump;
	GatesEngine::Input* input;
	PlayerCamera* mainCamera;
	float fuelValue;
	const float MAX_FUEL;
	const float CHARGE_FUEL;

	std::vector<Bullet*> bullets;
	bool isShotInterval;
	float shotInterval;

	std::vector<LockonTarget> targets;
	//std::vector<GatesEngine::GameObject*> targets;
	const int MAX_TARGET;
	int currentFrameTargetCount;

	int unuseBulletCount;
	const int MAX_HP;
	int hp;

	bool doubleTapKeys[4];
	bool isFirstTapKeys[4];
	float doubleTapKeysTime[4];
	GatesEngine::Math::Vector3 addVel;
	float decayTime;

	float moveVelDecayTime;

	bool normalAttackFlag;
	bool lockonAttackFlag;
	bool emitteAttackFlag;
	bool boostMoveFlag;
private:
	/// <summary>
	/// moveVel��jetVel�̕␳��vel�ɑ΂��ăZ�b�g����֐�
	/// </summary>
	void SetVelocity();

	/// <summary>
	/// �␳����vel���|�W�V�����ɉ��Z����֐�
	/// </summary>
	void SetPosition();

	/// <summary>
	/// mainCamera�̎��_���v���C���[�̉�]�s��ɔ��f����֐�
	/// </summary>
	void SetRotation();

	/// <summary>
	/// fuelValue���g�p����jetVel�ɑ΂���Vector3(0,const float,0)�����Z����֐�
	/// </summary>
	void UseJet();

	/// <summary>
	/// ��L��private�֐����g���Ĉړ����s���֐�
	/// </summary>
	void Move();

	/// <summary>
	/// �U��������֐�
	/// </summary>
	void Attack();

	/// <summary>
	/// ���b�N�I�������^�[�Q�b�g�ɑ΂��đ��U������֐�
	/// </summary>
	void LockOnAttack();

	/// <summary>
	/// �ЂƂ����܂�Œe�������A���b��ɔ��U����U���֐�
	/// </summary>
	void EmittionAttack();

	/// <summary>
	/// ���b�N�I�������G�����N���A����֐�
	/// </summary>
	void ClearTargets();

	/// <summary>
	/// �^�[�Q�b�g���X�g�̐[�x�\�[�g
	/// </summary>
	void SortTarget();

	void MergeSort(LockonTarget* data, size_t begin, size_t end, LockonTarget* work);

	void DecayVelocity(GatesEngine::Math::Vector3& vec,float& time,const float MAX_TIME);
public:
	PlayerBehaviour();
	~PlayerBehaviour();
	void Start() override;
	void Update() override;
	void OnDraw() override;
	void OnLateDraw() override;
	void OnCollision(GatesEngine::GameObject* other) override;
	void OnCollision(GatesEngine::Collider* hitCollider) override;
	void SetCamera(PlayerCamera* pCamera);
	PlayerCamera* GetSetCamera();
	void AddBullet(Bullet* newBullet);
	void AddTarget(GatesEngine::GameObject* other);
	int GetHp();
	bool GetNormalAttackFlag();
	bool GetLockonAttackFlag();
	bool GetEmitteAttackFlag();
	bool GetBoostMoveFlag();
};
