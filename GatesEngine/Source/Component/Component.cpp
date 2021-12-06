#include "..\..\Header\Component\Component.h"
#include "..\..\Header\GameObject\GameObject.h"
#include "..\..\Header\Component\Collider.h"

GatesEngine::Component::Component() :name("none"), tag("none"),isEnabled(true), gameObject(nullptr)
{
}

GatesEngine::Component::~Component()
{
}

void GatesEngine::Component::Start()
{
}

void GatesEngine::Component::Update()
{
}

void GatesEngine::Component::OnDraw()
{
}

void GatesEngine::Component::OnLateDraw()
{
}

void GatesEngine::Component::ExternalDraw()
{
}

void GatesEngine::Component::OnCollision(GameObject* other)
{
}

void GatesEngine::Component::OnCollision(Collider* hitCollider)
{
}

void GatesEngine::Component::SetGameObject(GameObject* gameObject)
{
	this->gameObject = gameObject;
}

void GatesEngine::Component::SetNameAndTag(const char* name, const char* tag)
{
	this->name = name;
	this->tag = tag;
}

void GatesEngine::Component::SetEnabled(bool flag)
{
	isEnabled = flag;
}

bool GatesEngine::Component::GetEnabled()
{
	return isEnabled;
}

GatesEngine::GameObject* GatesEngine::Component::GetGameObject()
{
	return gameObject;
}
