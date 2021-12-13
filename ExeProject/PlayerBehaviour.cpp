#include "PlayerBehaviour.h"
#include "Header/GameObject/GameObject.h"
#include "Header/Graphics/Graphics.h"
#include "NormalEnemyBehaviour.h"
#include "Header/Component/Collider.h"
#include "Header/Math/Vector3.h"
#include "Header/Graphics/Manager/ResourceManager.h"
#include "Header/Graphics/Camera2D.h"

void PlayerBehaviour::SetVelocity()
{
	const float G = 0.0f;
	const GatesEngine::Math::Vector3 MAX_VELOCITY = GatesEngine::Math::Vector3( 10 * 10,  10 * 10,   10 * 10);
	const GatesEngine::Math::Vector3 MIN_VELOCITY = GatesEngine::Math::Vector3(-10 * 10, -10 * 10, -10 * 10);

	jetVel -= GatesEngine::Math::Vector3(0, G, 0);

	// 各ベクトルの値を補正
	moveVel = GatesEngine::Math::Vector3::Max(moveVel, MAX_VELOCITY);
	moveVel = GatesEngine::Math::Vector3::Min(moveVel, MIN_VELOCITY);
	addVel = GatesEngine::Math::Vector3::Max(addVel, MAX_VELOCITY);
	addVel = GatesEngine::Math::Vector3::Min(addVel, MIN_VELOCITY);
	jetVel = GatesEngine::Math::Vector3::Max(jetVel, MAX_VELOCITY);
	jetVel = GatesEngine::Math::Vector3::Min(jetVel, MIN_VELOCITY);

	const float MAX_ADD_VEL_LENGTH = 1000;
	if (addVel.Length() > MAX_ADD_VEL_LENGTH)addVel = addVel.Normalize() * MAX_ADD_VEL_LENGTH;
	vel = moveVel + jetVel + addVel;
}

void PlayerBehaviour::SetPosition()
{
	gameObject->GetTransform()->position += vel;
}

void PlayerBehaviour::SetRotation()
{
	// カメラ向いている方向にプレイヤーモデルを回転させる
	GatesEngine::Math::Vector3 zRotate = mainCamera->GetRotation().GetAxis().z;
	gameObject->GetTransform()->rotation.y = atan2f(zRotate.x, zRotate.z);
}

void PlayerBehaviour::UseJet()
{
	// 1フレームに何ピクセル移動するか
	const float JET_FACTOR = 10;
	// 1秒間にfuelValueをどれだけ使用するか 消費量 * (1.0f / frameRate)
	const float USE_FUEL_VALUE = 1 * (1.0f / 144.0f);
	GatesEngine::Keyboard* keyboard = input->GetKeyboard();
	jetVel = {};
	if (keyboard->CheckHitKey(GatesEngine::Keys::SPACE))
	{
		if (fuelValue > 0)
		{
			fuelValue -= USE_FUEL_VALUE;
			jetVel.y += JET_FACTOR;
		}
	}
}

void PlayerBehaviour::Move()
{
	const float G = 0.25f;
	const float MOVE_SPEED = 5;
	const float DAMPING_FACTOR = 1;

	GatesEngine::Keyboard* keyboard = input->GetKeyboard();
	GatesEngine::Math::Axis playerAxis = mainCamera->GetRotation().GetAxis();
	GatesEngine::Math::Vector3 moveVector;

	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_DOUBLE_TAP_PERMIT_TIME = 1;

	for (int i = 0; i < 4; ++i)
	{
		if (doubleTapKeysTime[i] >= MAX_DOUBLE_TAP_PERMIT_TIME)
		{
			isFirstTapKeys[i] = false;
			doubleTapKeysTime[i] = 0;
		}
		if (isFirstTapKeys[i])
		{
			doubleTapKeysTime[i] += PER_FRAME;
		}
	}

	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::W) && isFirstTapKeys[0])
	{
		doubleTapKeys[0] = isFirstTapKeys[0];
	}
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::A) && isFirstTapKeys[1])
	{
		doubleTapKeys[1] = isFirstTapKeys[1];
	}
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::S) && isFirstTapKeys[2])
	{
		doubleTapKeys[2] = isFirstTapKeys[2];
	}
	if (input->GetKeyboard()->CheckPressTrigger(GatesEngine::Keys::D) && isFirstTapKeys[3])
	{
		doubleTapKeys[3] = isFirstTapKeys[3];
	}

	// 移動処理
	bool isInputMoveKey = false;
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::W))
	{
		isInputMoveKey = true;
		moveVector += playerAxis.z;
		isFirstTapKeys[0] = true;
	}
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::A))
	{
		isInputMoveKey = true;
		moveVector -= playerAxis.x;
		isFirstTapKeys[1] = true;
	}
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::S))
	{
		isInputMoveKey = true;
		moveVector -= playerAxis.z;
		isFirstTapKeys[2] = true;
	}
	if (input->GetKeyboard()->CheckHitKey(GatesEngine::Keys::D))
	{
		isInputMoveKey = true;
		moveVector += playerAxis.x;
		isFirstTapKeys[3] = true;
	}

	const float ADD_SPEED = 100;
	GatesEngine::Math::Vector3 addVector;
	if (doubleTapKeys[0])
	{
		addVector += playerAxis.z, doubleTapKeys[0] = false;
		decayTime = 0;
	}
	if (doubleTapKeys[1])
	{
		addVector -= playerAxis.x, doubleTapKeys[1] = false;
		decayTime = 0;
	}
	if (doubleTapKeys[2])
	{
		addVector -= playerAxis.z, doubleTapKeys[2] = false;
		decayTime = 0;
	}
	if (doubleTapKeys[3])
	{
		addVector += playerAxis.x, doubleTapKeys[3] = false;
		decayTime = 0;
	}

	float addVelLength = addVel.Length();
	const float MAX_DECAY_TIME = 1;
	if (addVelLength > 0)
	{
		decayTime += PER_FRAME;
		GatesEngine::Math::Vector3 decayVector = addVel.Normalize() * 2;
		addVel -= decayVector;
		if (decayTime > MAX_DECAY_TIME || addVel.Length() < decayVector.Length())
		{
			decayTime = 0;
			addVel = {};
		}
	}
	moveVel = moveVector.Normalize() * MOVE_SPEED;
	addVel += addVector.Normalize() * ADD_SPEED;

	UseJet();
	SetVelocity();
	SetRotation();
	SetPosition();
}

void PlayerBehaviour::Attack()
{
	GatesEngine::Math::Axis cameraAxis = mainCamera->GetRotation().GetAxis();

	bool isInputLeftClick = (input->GetMouse()->GetCheckHitButton(GatesEngine::MouseButtons::LEFT_CLICK)) ? true : false;
	bool isShot = false;
	unuseBulletCount = 0;

	const float PER_FRAME = 1.0f / 60.0f;
	const float MAX_SHOT_INTERVAL = 0.5f;

	if (isShotInterval)
	{
		shotInterval -= PER_FRAME;
	}

	if (shotInterval < 0)
	{
		isShotInterval = false;
		shotInterval = MAX_SHOT_INTERVAL;
	}


	// 使っていない弾を走査したのち移動ベクトルを設定する
	// その後の走査はプレイヤーの位置にセットする
	for (int i = 0; i < (int)bullets.size(); ++i)
	{
		if (!isShot && isInputLeftClick && !isShotInterval)
		{
			if (!(*bullets[i]).IsUse())
			{
				(*bullets[i]).Shot(cameraAxis.z.Normalize());
				isShot = true;
				isShotInterval = true;
			}
		}
		else
		{
			(*bullets[i]).SetPos(gameObject->GetTransform()->position);
		}

		if (!(*bullets[i]).IsUse())
		{
			++unuseBulletCount;
		}
	}
}

void PlayerBehaviour::LockOnAttack()
{
	if (currentFrameTargetCount <= 0)return;
	if (input->GetMouse()->GetCheckPressTrigger(GatesEngine::MouseButtons::RIGHT_CLICK))
	{
		int unusedBulletCount = 0;
		// 使っていない弾の総量を取得
		for (auto& b : bullets)
		{
			if (!b->IsUse())
			{
				++unusedBulletCount;
			}
		}
		int useBulletForOneEnemy = (int)bullets.size() / currentFrameTargetCount;
		const int MAX_USE_BULLET_FOR_ONE_ENEMY = 2;

		int useBulletCount = 0;
		for (auto& t : targets)
		{
			if (!t.GetIsLockon())break;
			GatesEngine::Math::Vector3 targetDir = t.GetTarget()->GetTransform()->position - gameObject->GetTransform()->position;
			targetDir = GatesEngine::Math::Vector3::Normalize(targetDir);
			for (auto& b : bullets)
			{
				if (b->IsUse())continue;
				GatesEngine::Math::Axis axis = mainCamera->GetRotation().GetAxis();
				b->SetTarget(t.GetTarget(), 0, axis, {2500,5000,2500});

				++useBulletCount;
				if (useBulletCount >= MAX_USE_BULLET_FOR_ONE_ENEMY || useBulletCount >= useBulletForOneEnemy)
				{
					useBulletCount = 0;
					break;
				}
			}
		}

		currentFrameTargetCount = 0;
		ClearTargets();
	}
}

void PlayerBehaviour::ClearTargets()
{
	for (auto& g : targets)
	{
		g.Initialize();
	}
}

PlayerBehaviour::PlayerBehaviour()
	: vel({})
	, isJump(false)
	, input(nullptr)
	, mainCamera(nullptr)
	, fuelValue(1000)
	, MAX_FUEL(1000)
	, CHARGE_FUEL(1)
	, MAX_TARGET(100)
	, currentFrameTargetCount(0)
	, unuseBulletCount(0)
	, MAX_HP(10)
	, hp(MAX_HP)
{
	mainCamera = new PlayerCamera();
	targets.resize(MAX_TARGET);
}

PlayerBehaviour::~PlayerBehaviour()
{
	delete mainCamera;
}

void PlayerBehaviour::Start()
{
	input = GatesEngine::Input::GetInstance();
	isJump = true;
	gameObject->GetTransform()->position.y = 1000;
	fuelValue = MAX_FUEL;
	hp = 10;

	for (int i = 0; i < 4; ++i)
	{
		isFirstTapKeys[i] = false;
		doubleTapKeysTime[i] = 0;
		doubleTapKeysTime[i] = 0;
	}

	isShotInterval = false;
	shotInterval = 0;
}

void PlayerBehaviour::Update()
{
	for (auto& t : targets)
	{
		t.Update();
	}
	Move();
	Attack();
	LockOnAttack();

	// カメラのポジション更新
	GatesEngine::Math::Vector3 pos = gameObject->GetTransform()->position;
	GatesEngine::Math::Axis pAxis = gameObject->GetTransform()->GetMatrix().GetAxis();
	GatesEngine::Math::Axis cAxis = mainCamera->GetRotation().GetAxis();
	mainCamera->SetPosition({ GatesEngine::Math::Vector3(pos.x,pos.y + 100,pos.z) - cAxis.z * 300 });
}

void PlayerBehaviour::OnDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	GatesEngine::ResourceManager::GetShaderManager()->GetShader("testMultiRTVShader")->Set();
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, gameObject->GetTransform()->GetMatrix());
	mainCamera->Set(2);
	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ {0,-1,0,0},{1,1,1,1} });
	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Cube")->Draw();

}

void PlayerBehaviour::OnLateDraw()
{
	GatesEngine::GraphicsDevice* graphicsDevice = gameObject->GetGraphicsDevice();

	//float persent = fuelValue / MAX_FUEL;
	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultSpriteShader")->Set();
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 100,persent * 1080,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 1920,1080/2,0 }));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	//graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();

	//// 残弾数の表示
	//float h = 1080.0f / (int)bullets.size();

	//for (int i = 0; i < unuseBulletCount; ++i)
	//{
	//	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 100,h,1 }) * GatesEngine::Math::Matrix4x4::Translate({ 0,(float)i * h,0 }));
	//	graphicsDevice->GetCBufferAllocater()->BindAndAttach(1, GatesEngine::Math::Vector4(1, 0, 0, 1));
	//	graphicsDevice->GetCBufferAllocater()->BindAndAttach(2, GatesEngine::Math::Matrix4x4::GetOrthographMatrix({ 1920,1080 }));
	//	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("2DPlane")->Draw();
	//}

	static float time = 0;
	time += 0.016f;
	GatesEngine::Math::Quaternion rotate = GatesEngine::Math::Quaternion({ {0,0,1},time });
	for (auto& t : targets)
	{
		if (!t.GetIsLockon())continue;
		float d = 1;
		if (d < 1)d = 1;
		float addScale = (t.GetMaxLockonTime() - t.GetLockTime()) / t.GetMaxLockonTime();
		d *= GatesEngine::Math::Easing::EaseOutQuad(addScale);
		GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultMeshShader")->Set();
		graphicsDevice->GetCBVSRVUAVHeap()->Set();
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, GatesEngine::Math::Matrix4x4::Scale({ 500 * d }) * GatesEngine::Math::Quaternion::Rotation(rotate) * mainCamera->GetRotation() * GatesEngine::Math::Matrix4x4::Translate({ t.GetTarget()->GetTransform()->position }));
		graphicsDevice->GetMainCamera()->Set(2);
		graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,0,1),GatesEngine::Math::Vector4(0,0,0,1) });
		GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();
	}

	//GatesEngine::Math::Vector3 center = gameObject->GetTransform()->position;
	//GatesEngine::Math::Vector3 size = 20;
	//rotate = GatesEngine::Math::Quaternion({ {0,1,0},0 });
	//GatesEngine::Math::Matrix4x4 scaleMatrix = GatesEngine::Math::Matrix4x4::Scale(size);
	//GatesEngine::Math::Matrix4x4 rotateMatrix = mainCamera->GetRotation();
	//GatesEngine::Math::Matrix4x4 posMatrix = GatesEngine::Math::Matrix4x4::Translate(center);

	//for (int i = 0; i < hp; ++i)
	//{
	//	GatesEngine::ResourceManager::GetShaderManager()->GetShader("DefaultMeshShader")->Set();

	//	graphicsDevice->GetCBVSRVUAVHeap()->Set();
	//	GatesEngine::Math::Vector3 offset = center + GatesEngine::Math::Vector3(0, gameObject->GetTransform()->scale.y * 2, 0);
	//	GatesEngine::Math::Vector3 spacePos = GatesEngine::Math::Vector3(0, 0, 0);
	//	GatesEngine::Math::Vector3 addPos = GatesEngine::Math::Vector3(size.x, 0, 0) * i;
	//	GatesEngine::Math::Vector3 fixPos = offset + addPos + spacePos - GatesEngine::Math::Vector3(size.x, 0, 0) * MAX_HP / 2;
	//	posMatrix = GatesEngine::Math::Matrix4x4::Translate(fixPos);
	//	graphicsDevice->GetCBufferAllocater()->BindAndAttach(0, scaleMatrix * rotateMatrix * posMatrix);
	//	mainCamera->Set(2);
	//	graphicsDevice->GetCBufferAllocater()->BindAndAttach(3, GatesEngine::B3{ GatesEngine::Math::Vector4(0,0,-1,1),GatesEngine::Math::Vector4(1,0,0,1) });
	//	GatesEngine::ResourceManager::GetMeshManager()->GetMesh("Plane")->Draw();
	//}
}

void PlayerBehaviour::OnCollision(GatesEngine::GameObject* other)
{
}

void PlayerBehaviour::OnCollision(GatesEngine::Collider* hitCollider)
{
	if (hitCollider->GetGameObject()->GetTag() == "block")
	{
		GatesEngine::Math::Vector3 otherOffSet = hitCollider->GetGameObject()->GetTransform()->position;
		GatesEngine::Math::Vector3 otherColliderPos = otherOffSet + hitCollider->GetTransform()->position;
		GatesEngine::Math::Vector3 otherGameObjectSize = hitCollider->GetGameObject()->GetTransform()->scale;
		GatesEngine::Math::Vector3 otherColliderSize = hitCollider->GetTransform()->scale;

		GatesEngine::Math::Vector3 offSet = gameObject->GetTransform()->position;
		GatesEngine::Math::Vector3 colliderPos = otherOffSet + gameObject->GetCollider()->GetTransform()->position;
		GatesEngine::Math::Vector3 gameObjectSize = gameObject->GetTransform()->scale;
		GatesEngine::Math::Vector3 colliderSize = gameObject->GetCollider()->GetTransform()->scale;

		gameObject->GetTransform()->position.y = otherColliderPos.y + (otherGameObjectSize.y * otherColliderSize.y / 2 + gameObjectSize.y * colliderSize.y / 2);
		jetVel = {};
		isJump = false;
		if (fuelValue >= MAX_FUEL)fuelValue = MAX_FUEL;
		fuelValue += CHARGE_FUEL;
	}

	//if (hitCollider->GetGameObject()->GetTag() == "enemyBullet")
	//{
	//	--hp;
	//}
}

void PlayerBehaviour::SetCamera(PlayerCamera* pCamera)
{
	mainCamera = pCamera;
}

PlayerCamera* PlayerBehaviour::GetSetCamera()
{
	return mainCamera;
}

void PlayerBehaviour::AddBullet(PlayerBullet* newBullet)
{
	bullets.push_back(newBullet);
}

void PlayerBehaviour::AddTarget(GatesEngine::GameObject* other)
{
	for (auto& g : targets)
	{
		// ロックオン中の敵がいた場合スキップ
		if (g.GetTarget() == other)
		{
			g.Initialize();
			g.SetLockonTarget(other);
			return;
		}

		// 登録されてなければターゲットに設定
		if (!g.GetTarget())
		{
			g.Initialize();
			g.SetLockonTarget(other);
			++currentFrameTargetCount;
			break;
		}
	}


}

int PlayerBehaviour::GetHp()
{
	return hp;
}
