#pragma once
#include "Header/Math/Vector2.h"

enum class ButtonUIType
{
	START,
	EXIT,
};

/// <summary>
/// 2DUI�Ɏg���N���X
/// </summary>
class ButtonUI
{
protected:
	using Vector2 = GatesEngine::Math::Vector2;

	Vector2 scale;
	Vector2 pos;

	bool isCenter;
	bool isSelect;

	float alpha;

	ButtonUIType type;
public:

	/// <summary>
	/// �{�^���̑傫����Ԃ��֐�
	/// </summary>
	/// <returns>�{�^���̑傫��</returns>
	const Vector2& GetScale();

	/// <summary>
	/// �{�^���̈ʒu��Ԃ��֐�
	/// </summary>
	/// <returns>�{�^���̈ʒu</returns>
	const Vector2& GetPos();

	/// <summary>
	/// �{�^���̊�_�����ォ���S���m�F����֐�
	/// </summary>
	/// <returns>����</returns>
	bool GetIsCenter();

	/// <summary>
	/// �{�^�����I������Ă��邩�m�F����֐�
	/// </summary>
	/// <returns>����</returns>
	bool GetIsSelect();

	/// <summary>
	/// UI�̓����x���擾����֐�
	/// </summary>
	/// <returns>UI�̓����x</returns>
	float GetAlpha();

	/// <summary>
	/// �{�^���̎�ނ��擾����֐�
	/// </summary>
	/// <returns>�{�^���̎��</returns>
	ButtonUIType GetType();

	/// <summary>
	/// �{�^���̑傫����ݒ肷��֐�
	/// </summary>
	/// <param name="setscale">�ݒ肷��傫��</param>
	void SetScale(const Vector2& setscale);

	/// <summary>
	/// �{�^���̈ʒu��ݒ肷��֐�
	/// </summary>
	/// <param name="setpos">�ݒ肷��ʒu</param>
	void SetPos(const Vector2& setpos);

	/// <summary>
	/// �{�^���̊�_������ɂ��邩���S�ɂ��邩�ݒ肷��֐�
	/// </summary>
	/// <param name="flag">����</param>
	void SetIsCenterFlag(bool flag);

	/// <summary>
	/// �{�^�����I������Ă��邩�ݒ肷��֐�
	/// </summary>
	/// <param name="flag">����</param>
	void SetIsSelect(bool flag);

	/// <summary>
	/// UI�̓����x��ݒ肷��֐�
	/// </summary>
	/// <param name="value">�ݒ�l</param>
	void SetAlpha(float value);

	/// <summary>
	/// �{�^���̎�ނ�ݒ肷��֐�
	/// </summary>
	/// <param name="buttonType">�ݒ肷��^�C�v</param>
	void SetType(ButtonUIType buttonType);
};

