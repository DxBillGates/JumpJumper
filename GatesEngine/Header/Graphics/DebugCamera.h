#pragma once
#include "Camera3D.h"

namespace GatesEngine
{
	class Input;
	//�f�o�b�O���Ɏg�����R�ړ��\�ȃJ������z��Ƃ����N���X�A�}�E�X���g���ăJ�����̎��_�ړ���A
	//�L�[�{�[�h���g�p���Ĉړ����\
	class DebugCamera : public Camera3D
	{
	protected:
		Input* input;
		float moveSpeed;
	public:
		DebugCamera();
		virtual ~DebugCamera();
		void Initialize() override;
		void Update() override;
	};
}
