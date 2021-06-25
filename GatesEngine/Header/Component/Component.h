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
		GameObject* gameObject;
	public:
		Component();
		virtual ~Component();
		virtual void Start();
		virtual void Update();
		virtual void OnDraw();
		void SetGameObject(GameObject* gameObject);
		void SetNameAndTag(const char* name, const char* tag);
	};
}
