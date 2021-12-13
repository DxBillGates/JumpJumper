#pragma once
#include "Header/Math/Vector3.h"
class Enemy
{
protected:
	GatesEngine::Math::Vector3 iPos;
	GatesEngine::Math::Vector3 pos;
	GatesEngine::Math::Vector3 vel;
	GatesEngine::Math::Vector3 acc;
	GatesEngine::Math::Vector3 target;
	float setTime;
	bool isUse;
	bool isSetting;
	bool isTarget;

	bool isSpawning;
	float spawnTime;
public:
	Enemy();
	~Enemy();
	void Initialize();
	void Update();

	/// <summary>
	/// ���W���擾����֐�
	/// </summary>
	/// <returns>���W</returns>
	GatesEngine::Math::Vector3 GetPosition();

	/// <summary>
	/// �g�p�����m�F����֐�
	/// </summary>
	/// <returns>����</returns>
	bool GetIsUse();

	/// <summary>
	/// 
	/// </summary>
	/// <returns>����</returns>
	bool GetIsSetting();

	/// <summary>
	/// �^�[�Q�b�g���ݒ肳��Ă��邩�m�F����֐�
	/// </summary>
	/// <returns>����</returns>
	bool GetIsTarget();

	/// <summary>
	/// �ʒu��ݒ肷��֐�
	/// </summary>
	/// <param name="pos">�Z�b�g������W</param>
	void SetPosition(const GatesEngine::Math::Vector3& pos);

	/// <summary>
	/// �����o�ϐ���vel��ݒ肷��֐�
	/// </summary>
	/// <param name="vel">�ݒ肷��l</param>
	void SetVelocity(const GatesEngine::Math::Vector3& vel);

	/// <summary>
	/// �ݒ�ɂ����鎞�Ԃ�ݒ肷��
	/// </summary>
	/// <param name="value">�ݒ肷��l</param>
	void SetTime(float value);

	/// <summary>
	/// �g�p���Ă��邩���m�F����ϐ���ݒ肷��֐�
	/// </summary>
	/// <param name="flag">�ݒ肷��l</param>
	void SetIsUse(bool flag);

	/// <summary>
	/// �z�[�~���O�̂悤�ȋ�����ݒ肷��֐�
	/// </summary>
	/// <param name="targetPos"></param>
	void SetTarget(const GatesEngine::Math::Vector3& targetPos, const GatesEngine::Math::Vector3& addVector);
};

