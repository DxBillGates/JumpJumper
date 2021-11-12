#pragma once
#include "..\GameObject\GameObjectManager.h"
#include "..\Collision\CollisionManager.h"

namespace GatesEngine
{
	class Application;
	class GraphicsDevice;
	class Scene
	{
	protected:
		Application* app;
		GraphicsDevice* graphicsDevice;
		GameObjectManager gameObjectManager;
		CollisionManager collisionManager;
		const char* sceneName;
		bool changeNextSceneFlag;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		Scene();

		/// <summary>
		/// �V�[���ɑ΂��Ė��O��ݒ肷��R���X�g���N�^
		/// </summary>
		/// <param name="sceneName">���O</param>
		Scene(const char* sceneName);

		/// <summary>
		/// �V�[���ɑ΂��Ė��O�Ƃ��̃A�v���P�[�V�����̃|�C���^��ݒ肷��R���X�g���N�^
		/// </summary>
		/// <param name="sceneName">���O</param>
		/// <param name="app">�A�v���P�[�V�����̃|�C���^</param>
		Scene(const char* sceneName, Application* app);

		/// <summary>
		/// ���z�f�X�g���N�^
		/// </summary>
		virtual ~Scene();

		/// <summary>
		/// �V�[���̏�����������̏������z�֐�
		/// </summary>
		virtual void Initialize() = 0;

		/// <summary>
		/// �V�[���̍X�V������̏������z�֐�
		/// </summary>
		virtual void Update() = 0;

		/// <summary>
		/// �V�[���̕`�������̏������z�֐�
		/// </summary>
		virtual void Draw() = 0;

		/// <summary>
		/// �V�[���̒x���`�������̉��z�֐�
		/// </summary>
		virtual void LateDraw() {}

		/// <summary>
		/// �V�[���̖��O��Ԃ��֐�
		/// </summary>
		/// <returns>�V�[���̖��O</returns>
		const char* GetSceneName();

		/// <summary>
		/// �V�[�������̃V�[���ɕύX���邩�ݒ肷��֐�
		/// </summary>
		/// <returns>����</returns>
		bool IsChangeScene();

		/// <summary>
		/// �V�[���ɑ��݂���Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�Q�[���I�u�W�F�N�g�}�l�[�W���[�̃|�C���^</returns>
		GameObjectManager* GetGameObjectManager();
	};
}
