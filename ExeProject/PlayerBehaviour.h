#pragma once
#include "Header/Component/Behaviour.h"
#include "Header/Math/Math.h"
#include "Header/Input/Input.h"
#include "Header/Graphics/Camera.h"
#include "PlayerBullet.h"
#include "PlayerCamera.h"

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

	std::vector<PlayerBullet*> bullets;
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
	void AddBullet(PlayerBullet* newBullet);
};
