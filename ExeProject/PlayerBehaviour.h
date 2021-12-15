#pragma once
#include "Header/Component/Behaviour.h"
#include "LockonTarget.h"
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

	/// <summary>
	/// ロックオンしたターゲットに対して総攻撃する関数
	/// </summary>
	void LockOnAttack();

	/// <summary>
	/// ひとかたまりで弾を撃ち、数秒後に爆散する攻撃関数
	/// </summary>
	void EmittionAttack();

	/// <summary>
	/// ロックオンした敵情報をクリアする関数
	/// </summary>
	void ClearTargets();
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
};
