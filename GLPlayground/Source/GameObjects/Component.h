#pragma once

enum class ComponentsType : int
{
	Base,
	Transform,
	Renderable,
	Text
};

class Object;
class Scene;

class Component
{
public:
	Component();
	virtual ~Component();
	
	virtual void Start() = 0;
	virtual void Update(float DeltaTime) = 0;
	virtual void LateUpdate(float DeltaTime) = 0;
	virtual void End() = 0;
	virtual void OnAttached(Object * AttachedTo);
	virtual void OnDetached();

	virtual ComponentsType GetType()
	{
		return ComponentsType::Base;
	}

	inline void SetLogicScene(Scene * NewLogicScene)
	{
		LogicScene = NewLogicScene;
	}

protected:
	Object * Owner;

protected:
	//Can and sometimes will be nullptr
	Scene * LogicScene;
};

