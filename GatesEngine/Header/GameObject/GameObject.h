#pragma once
#include "..\Component\Behaviour.h"
#include "..\Math\Transform.h"
#include <vector>

namespace GatesEngine
{
	class GraphicsDevice;
	class GameObject
	{
	private:
		std::vector<Component*> components;
		std::vector<Behaviour*> behaviors;
		const char* name;
		const char* tag;
		GraphicsDevice* graphicsDevice;
		Transform transform;
	public:
		GameObject();
		GameObject(const char* name, const char* tag = "none");
		~GameObject();
		void Start();
		void Update();
		void Draw();
		void SetGraphicsDevice(GraphicsDevice* pGraphicsDevice);
		GraphicsDevice* GetGraphicsDevice();
		Transform* GetTransform();

		template <typename T>
		T* AddComponent();
		template <typename T>
		T* AddBehavior();
		template <typename T>
		T* GetComponent();
	};
	template<typename T>
	inline T* GameObject::AddComponent()
	{
		T* t = new T();
		//Component�N���X����h���������̂��`�F�b�N
		Component* pComponent = static_cast<Component*>(t);
		if (pComponent != nullptr)
		{
			components.push_back(pComponent);
			pComponent->SetGameObject(this);
			return t;
		}
		return nullptr;
	}
	template<typename T>
	inline T* GameObject::AddBehavior()
	{
		T* t = new T();
		//Component�N���X����h���������̂��`�F�b�N
		Behaviour* pComponent = static_cast<Behaviour*>(t);
		if (pComponent != nullptr)
		{
			behaviors.push_back(pComponent);
			pComponent->SetGameObject(this);
			return t;
		}
		return nullptr;
	}
	template<typename T>
	inline T* GameObject::GetComponent()
	{
		for (int i = 0; i < (int)components.size(); i++)
		{
			T* component = dynamic_cast<T*>(components[i]);
			if (component != nullptr)
			{
				return component;
			}
		}
		for (int i = 0; i < (int)behaviors.size(); ++i)
		{
			T* component = dynamic_cast<T*>(behaviors[i]);
			if (component != nullptr)
			{
				return component;
			}
		}
		return nullptr;
	}
}