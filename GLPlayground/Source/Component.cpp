#include "Component.h"
#include "GameObject.h"
#include "Scene.h"

Component::Component()
{
}


Component::~Component()
{
}

void Component::OnAttached(GameObject * AttachedTo)
{
	Owner = AttachedTo;
}

void Component::OnDetached()
{
	Owner = nullptr;
}
