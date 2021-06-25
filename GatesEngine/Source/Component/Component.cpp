#include "..\..\Header\Component\Component.h"
#include "..\..\Header\GameObject\GameObject.h"

GatesEngine::Component::Component() :name("none"), tag("none"), gameObject(nullptr)
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

void GatesEngine::Component::OnCollision(GameObject* other)
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
