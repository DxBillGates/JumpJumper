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
	/// moveVelとjetVelの補正とvelに対してセットする関数
	/// </summary>
	void SetVelocity();

	/// <summary>
	/// 補正したvelをポジションに加算する関数
	/// </summary>
	void SetPosition();

	/// <summary>
	/// mainCameraの視点をプレイヤーの回転行列に反映する関数
	/// </summary>
	void SetRotation();

	/// <summary>
	/// fuelValueを使用してjetVelに対してVector3(0,const float,0)を加算する関数
	/// </summary>
	void UseJet();

	/// <summary>
	/// 上記のprivate関数を使って移動を行う関数
	/// </summary>
	void Move();

	/// <summary>
	/// 攻撃をする関数
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
