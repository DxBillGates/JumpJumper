#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Component\Component.h"
#include "..\..\Header\Component\Behaviour.h"

GatesEngine::GameObject::GameObject():name("none"),tag("none")
{
}

GatesEngine::GameObject::GameObject(const char* name, const char* tag):name(name),tag(tag)
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
	for (auto itr = components.begin(); itr != components.end(); ++itr)
	{
		(*itr)->Update();
	}
	for (auto itr = behaviors.begin(); itr != behaviors.end(); ++itr)
	{
		(*itr)->Update();
	}
}

void GatesEngine::GameObject::Draw()
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

void GatesEngine::GameObject::SetGraphicsDevice(GraphicsDevice* pGraphicsDevice)
{
	graphicsDevice = pGraphicsDevice;
}

GatesEngine::GraphicsDevice* GatesEngine::GameObject::GetGraphicsDevice()
{
	return graphicsDevice;
}

GatesEngine::Transform* GatesEngine::GameObject::GetTransform()
{
	return &transform;
}
