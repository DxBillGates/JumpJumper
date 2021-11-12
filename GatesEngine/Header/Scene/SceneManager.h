#pragma once
#include <vector>
#include "Scene.h"
namespace GatesEngine
{
	class SceneManager
	{
	private:
		std::vector<Scene*> scenes;
		Scene* currentScene;
		Scene* beforeScene;
	protected:
	public:
		/// <summary>
		/// �V���O���g���̃C���X�^���X�𐶐����ĕԂ��֐�
		/// </summary>
		/// <returns>�V���O���g���̃C���X�^���X�̃|�C���^</returns>
		static SceneManager* GetInstance();

		/// <summary>
		/// �V�[����ǉ�����֐�
		/// </summary>
		/// <param name="newScene">�ǉ�����V�[���̖��O</param>
		void AddScene(Scene* newScene);

		/// <summary>
		/// �V�[����ύX����֐�
		/// </summary>
		/// <param name="sceneName">�ύX��̃V�[���̖��O</param>
		void ChangeScene(const char* sceneName);

		/// <summary>
		/// �V�[����ύX������ɃV�[���̏��������s��Ȃ��֐�
		/// </summary>
		/// <param name="sceneName">�ύX��̃V�[���̖��O</param>
		void ChangeSceneWithoutInitialize(const char* sceneName);

		/// <summary>
		/// ���݂̃V�[���̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>���݂̃V�[���̃|�C���^</returns>
		Scene* GetCurrentScene();

		/// <summary>
		/// �O�̃V�[���̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <returns>�O�̃V�[���̃|�C���^</returns>
		Scene* GetBeforeScene();

		/// <summary>
		/// �w�肵���V�[���̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <param name="sceneName">�w�肷��V�[���̖��O</param>
		/// <returns>�w�肵���V�[���̃|�C���^</returns>
		Scene* GetScene(const char* sceneName);

		/// <summary>
		/// ���݂̃V�[��������������֐�
		/// </summary>
		void Initialize();

		/// <summary>
		/// ���݂̃V�[�����X�V����֐�
		/// </summary>
		void Update();

		/// <summary>
		/// ���݂̃V�[����`�悷��֐�
		/// </summary>
		void Draw();

		/// <summary>
		/// ���݂̃V�[����x���`�悷��֐�
		/// </summary>
		void LateDraw();
	private:
		SceneManager();
		SceneManager(const SceneManager&) = delete;
		~SceneManager();
		SceneManager& operator=(const SceneManager&) = delete;
	};
}
