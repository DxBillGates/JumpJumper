#pragma once
#include "GameObject.h"
namespace GatesEngine
{
	class GameObjectManager
	{
	private:
		std::vector<GameObject*> gameObjects;
	public:
		/// <summary>
		/// �R���X�g���N�^
		/// </summary>
		GameObjectManager();
		~GameObjectManager();

		/// <summary>
		/// �Q�[���I�u�W�F�N�g��ǉ�����֐�
		/// </summary>
		/// <param name="newGameObject">�ǉ�����Q�[���I�u�W�F�N�g�̃|�C���^</param>
		/// <returns>�ǉ������Q�[���I�u�W�F�N�g�̃|�C���^</returns>
		GameObject* Add(GameObject* newGameObject);

		/// <summary>
		/// ���O���A�^�O���w�肵�ăQ�[���I�u�W�F�N�g�����݂��邩�������Ă��̃Q�[���I�u�W�F�N�g�̃|�C���^��Ԃ��֐�
		/// </summary>
		/// <param name="name">�w�肷�閼�O���</param>
		/// <param name="tag">�w�肷��^�O</param>
		/// <returns>�������Č��������炻�̃Q�[���I�u�W�F�N�g�̃|�C���^�A������Ȃ������ꍇ��nullptr</returns>
		GameObject* Find(const char* name, const char* tag = nullptr);

		/// <summary>
		/// �o�^����Ă���Q�[���I�u�W�F�N�g�̏�����������֐�
		/// </summary>
		void Start();

		/// <summary>
		/// �o�^����Ă���Q�[���I�u�W�F�N�g�̍X�V������֐�
		/// </summary>
		void Update();

		/// <summary>
		/// �o�^����Ă���Q�[���I�u�W�F�N�g�̕`�������֐�
		/// </summary>
		void Draw();

		/// <summary>
		/// �o�^����Ă���Q�[���I�u�W�F�N�g�̒x���`�������֐�
		/// </summary>
		void LateDraw();
	};
}
