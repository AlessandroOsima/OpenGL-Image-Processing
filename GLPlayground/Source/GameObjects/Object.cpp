#include "stdafx.h"
#include "GameObjects/Object.h"
#include "Scene.h"


Object::Object()
{
}


Object::~Object()
{
}

unsigned int Object::AddComponent(std::unique_ptr<Component> && ComponentToAdd)
{
	Components.push_back(std::move(ComponentToAdd));
	ComponentLocation loc = Components.size() - 1;

	Components[loc]->OnAttached(this);
	Components[loc]->SetLogicScene(LogicScene);
	Components[loc]->Start();

	return Components.size() - 1;
}

void Object::RemoveComponent(ComponentLocation Location)
{
	if (Location >= Components.size())
	{
		return;
	}

	Components[Location]->OnDetached();
	Components[Location]->End();
	Components.erase(Components.begin() + Location);
}

unsigned int Object::GetLocation(const std::unique_ptr<Component> & ComponentToLocate)
{
	std::vector<std::unique_ptr<Component>>::iterator location = std::find(Components.begin(), Components.end(), ComponentToLocate);
	return  location - Components.begin();
}

Component * Object::GetComponentAtLocation(ComponentLocation Location)
{
	//static std::unique_ptr<Component> nullReturn = std::unique_ptr<Component>(nullptr);

	if (Location >= Components.size())
	{
		return nullptr;
	}

	return Components[Location].get();
}

std::vector<Component *>  Object::GetComponentsOfType(ComponentsType Type)
{
	std::vector<Component *> vec;

	for (auto & Component : Components)
	{
		if (Component->GetType() == Type)
		{
			vec.push_back(Component.get());
		}
	}

	return vec;
}

Component * Object::GetComponentOfType(ComponentsType Type)
{
	for (auto & Component : Components)
	{
		if (Component->GetType() == Type)
		{
			return Component.get();
		}
	}

	return nullptr;
}

void Object::Start()
{
	for (auto & Component : Components)
	{
		Component->Start();
	}
}

void Object::Update(float DeltaTime)
{
	for (auto & Component : Components)
	{
		Component->Update(DeltaTime);
	}

	for (auto & Component : Components)
	{
		Component->LateUpdate(DeltaTime);
	}
}

void Object::End()
{
	for (auto & Component : Components)
	{
		Component->End();
	}
}

