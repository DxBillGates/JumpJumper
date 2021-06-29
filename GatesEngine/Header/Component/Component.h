#pragma once

namespace GatesEngine
{
	class GameObject;
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
		virtual void OnCollision(GameObject* other);
		void SetGameObject(GameObject* gameObject);
		void SetNameAndTag(const char* name, const char* tag);
		void SetEnabled(bool flag);
		bool GetEnabled();
	};
}
