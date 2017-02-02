#include "stdafx.h"
#include "Component.h"
#include "Object.h"
#include "Scene.h"

Component::Component()
{
}


Component::~Component()
{
}

void Component::OnAttached(Object * AttachedTo)
{
	Owner = AttachedTo;
}

void Component::OnDetached()
{
	Owner = nullptr;
}
