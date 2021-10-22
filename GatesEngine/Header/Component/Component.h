#pragma once

namespace GatesEngine
{
	class GameObject;
	class Collider;
	class Component
	{
	private:
	protected:
		const char* name;
		const char* tag;
		bool isEnabled;
		GameObject* gameObject;
	public:
		Component();
		virtual ~Component();
		virtual void Start();
		virtual void Update();
		virtual void OnDraw();
		virtual void OnLateDraw();
		virtual void OnCollision(GameObject* other);
		virtual void OnCollision(Collider* hitCollider);
		void SetGameObject(GameObject* gameObject);
		void SetNameAndTag(const char* name, const char* tag);
		void SetEnabled(bool flag);
		bool GetEnabled();
		GameObject* GetGameObject();
	};
}
