#pragma once
#include "..\..\Header\Math\Math.h"
#include "..\..\Header\Window\Window.h"
#include "..\..\Header\Input\Input.h"
#include "GraphicsDevice.h"
#include "CBufferStruct.h"
namespace GatesEngine
{
	//��{�ƂȂ�J�����N���X�A��{��_�J�����ɂ����g���Ȃ�
	class Camera
	{
	protected:
		GraphicsDevice* pGraphicsDevice;
		Window* pMainWindow;

		Math::Matrix4x4 projectionMatrix;
		float nearClip;
		float farClip;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Camera();

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~Camera() {}

		/// <summary>
		/// ���z�������֐�
		/// </summary>
		virtual void Initialize() {}

		/// <summary>
		/// ���z�X�V�֐�
		/// </summary>
		virtual void Update() {}
		
		/// <summary>
		/// ���zGPU�Z�b�g�֐�
		/// </summary>
		/// <param name="setDescriptorIndex">�Z�b�g���郌�W�X�^�ԍ�</param>
		virtual void Set(int setDescriptorIndex) {}

		/// <summary>
		/// �A�v���P�[�V�����Ŏg�p���Ă���GraphicsDevice�̃|�C���^���Z�b�g����֐�
		/// </summary>
		/// <param name="gDevice">GraphicsDevice�̃|�C���^</param>
		void SetGraphicsDevice(GraphicsDevice* gDevice);

		/// <summary>
		/// �A�v���P�[�V�����Ŏg�p���Ă���MainWindow�̃|�C���^
		/// </summary>
		/// <param name="window">Window�̃|�C���^</param>
		void SetMainWindow(Window* window);

		/// <summary>
		/// �v���W�F�N�V�����s��̐ݒ�
		/// </summary>
		/// <param name="matrix">�v���W�F�N�V�����s��</param>
		void SetProjection(const Math::Matrix4x4& matrix);

		/// <summary>
		/// �j�A�\�N���b�v�ƃt�@�[�N���b�v�̐ݒ�
		/// </summary>
		/// <param name="n">�j�A�\�N���b�v�̒l</param>
		/// <param name="f">�t�@�[�N���b�v�̒l</param>
		void SetClip(float n = 1, float f = 10000);
	};
}