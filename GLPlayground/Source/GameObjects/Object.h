#pragma once
#include <vector>
#include <memory>
#include "GameObjects/Component.h"

using ComponentLocation = unsigned int;
class Scene;

class Object
{
public:
	Object();
	virtual ~Object();

	unsigned int AddComponent(std::unique_ptr<Component> && ComponentToAdd);
	void RemoveComponent(ComponentLocation Location);

	unsigned int GetLocation(const std::unique_ptr<Component> & ComponentToLocate);
	Component * GetComponentAtLocation(ComponentLocation Location);

	std::vector<Component *> GetComponentsOfType(ComponentsType Type);
	Component * GetComponentOfType(ComponentsType Type);

	virtual void Start();
	virtual void Update(float DeltaTime);
	virtual void End();

	inline void SetLogicScene(Scene * NewLogicScene)
	{
		LogicScene = NewLogicScene;
	}


private:
	std::vector<std::unique_ptr<Component>> Components;

	//Can and sometimes will be nullptr
	Scene * LogicScene;
};

