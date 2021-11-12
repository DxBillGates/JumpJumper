#pragma once
#include "..\..\Header\Math\Vector2.h"
#include "..\..\Header\Util\Timer.h"
#include "..\Window\Window.h"
#include "..\Input\Input.h"
#include "..\..\Header\Scene\SceneManager.h"
#include "..\..\Header\GameObject\GameObjectManager.h"
#include "..\Graphics\GraphicsDevice.h"
#include "..\Graphics\CBuffer.h"
#include "..\Graphics\Camera.h"

namespace GatesEngine
{
	class Application
	{
	private:
	protected:
		Util::Timer timer;
		Window mainWindow;
		Input* input;
		SceneManager* sceneManager;
		GameObjectManager gameObjectManager;
		GraphicsDevice graphicsDevice;
		CBuffer<B3> worldLightInfo;

		Camera* mainCamera;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Application();

		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		/// <param name="wSize">Window�̃T�C�Y</param>
		/// <param name="title">Window�̃^�C�g��</param>
		Application(const Math::Vector2& wSize, const char* title);

		/// <summary>
		/// �f�X�g���N�^
		/// </summary>
		virtual ~Application();

		/// <summary>
		/// �A�Z�b�g�̃��[�h�����鉼�z�֐�
		/// </summary>
		/// <returns>����</returns>
		virtual bool LoadContents();

		/// <summary>
		/// �A�v���P�[�V�����̏��������s�����z�֐�
		/// </summary>
		/// <returns>����</returns>
		virtual bool Initialize();

		/// <summary>
		/// �A�v���P�[�V�����̍X�V���s�����z�֐�
		/// </summary>
		/// <returns>����</returns>
		virtual bool Update();

		/// <summary>
		/// �A�v���P�[�V�����̕`����s�����z�֐�
		/// </summary>
		/// <returns>����</returns>
		virtual bool Draw();

		/// <summary>
		/// LoadContents�AInitialize����x�����s�����t���[��Update��Draw���s�����z�֐�
		/// </summary>
		virtual void Run();

		/// <summary>
		/// �Q�[�����o�ߎ��Ԃ�t���[�����Ԃ�ێ�����N���X�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>Timer�̃|�C���^��Ԃ�</returns>
		Util::Timer* GetTimer();

		/// <summary>
		/// �V�[���}�l�[�W���[�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�V�[���}�l�[�W���[�̃|�C���^��Ԃ�</returns>
		SceneManager* GetSceneManager();

		/// <summary>
		/// GraphicsDevice�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>GraphicsDevice�̃|�C���^��Ԃ�</returns>
		GraphicsDevice* GetGraphicsDevice();

		/// <summary>
		/// ���̃A�v���P�[�V�����Ŏg�p���Ă���Window�̏���ێ������N���X�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�g�p���Ă���Window�̏���ێ������N���X�̃|�C���^</returns>
		Window* GetWindow();

		/// <summary>
		/// �L�[�{�[�h��}�E�X�̏���ێ������N���X�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�L�[�{�[�h��}�E�X�̏���ێ������N���X�̃|�C���^��Ԃ�</returns>
		Input* GetInput();

		/// <summary>
		/// GraphicsDevice�̐ݒ肳��Ă���J�����̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�g���Ă���J�����̃|�C���^��Ԃ�</returns>
		Camera* GetMainCamera();
	};
}