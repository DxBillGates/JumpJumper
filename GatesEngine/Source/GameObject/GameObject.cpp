#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Component\Component.h"
#include "..\..\Header\Component\Behaviour.h"

GatesEngine::GameObject::GameObject() :GameObject("none", "none")
{
}

GatesEngine::GameObject::GameObject(const char* name, const char* tag)
	: name(name)
	, tag(tag)
	, graphicsDevice(nullptr)
	, transform({})
	, collider(nullptr)
	, enabled(true)
{
}

GatesEngine::GameObject::~GameObject()
{
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		delete (*itr);
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		delete (*itr);
	}
	components.clear();
	behaviors.clear();
}

void GatesEngine::GameObject::Start()
{
	enabled = true;
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		(*itr)->Start();
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		(*itr)->Start();
	}
}

void GatesEngine::GameObject::Update()
{
	if (enabled)
	{
		for (auto itr = components.begin(); itr != components.end(); ++itr)
		{
			if ((*itr)->GetEnabled())(*itr)->Update();
		}
		for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
		{
			if ((*itr)->GetEnabled())(*itr)->Update();
		}
	}
}

void GatesEngine::GameObject::Draw()
{
	if (enabled)
	{
		for (auto itr = components.begin(); itr != components.end(); ++itr)
		{
			(*itr)->OnDraw();
		}
		for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
		{
			(*itr)->OnDraw();
		}
	}
}

void GatesEngine::GameObject::LateDraw()
{
	if (enabled)
	{
		for (auto itr = components.begin(); itr != components.end(); ++itr)
		{
			(*itr)->OnLateDraw();
		}
		for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
		{
			(*itr)->OnLateDraw();
		}
	}
}

void GatesEngine::GameObject::Collision(GameObject* other)
{
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		(*itr)->OnCollision(other);
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		(*itr)->OnCollision(other);
	}
}

void GatesEngine::GameObject::Collision(Collider* hitCollider)
{
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		(*itr)->OnCollision(hitCollider);
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		(*itr)->OnCollision(hitCollider);
	}
}

void GatesEngine::GameObject::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}

void GatesEngine::GameObject::SetCollider()
{
	collider = GetComponent<Collider>();
}

void GatesEngine::GameObject::SetName(const char* sName)
{
	name = sName;
}

void GatesEngine::GameObject::SetTag(const char* sTag)
{
	tag = sTag;
}

const char* GatesEngine::GameObject::GetName()
{
	return name;
}

const char* GatesEngine::GameObject::GetTag()
{
	return tag;
}

GatesEngine::GraphicsDevice* GatesEngine::GameObject::GetGraphicsDevice()
{
	return graphicsDevice;
}

GatesEngine::Transform* GatesEngine::GameObject::GetTransform()
{
	return &transform;
}

GatesEngine::Collider* GatesEngine::GameObject::GetCollider()
{
	return collider;
}

void GatesEngine::GameObject::SetEnabled(bool flag)
{
	enabled = flag;
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		(*itr)->SetEnabled(enabled);
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		(*itr)->SetEnabled(enabled);
	}
}

bool GatesEngine::GameObject::GetEnabled()
{
	return enabled;
}
