#pragma once
#include "Component.h"
#include <glm/glm.hpp>

class Transform : public Component
{
public:
	Transform();
	virtual ~Transform();

	void Start() override;
	void Update(float DeltaTime) override;
	void LateUpdate(float DeltaTime) override;
	void End() override;

	virtual ComponentsType GetType()
	{
		return ComponentsType::Transform;
	}

	inline glm::mat4 GetRotate() const 
	{
		return Rotate;
	}

	inline glm::mat4 GetTranslate() const
	{
		return Translate;
	}

	inline glm::mat4 GetScale() const
	{
		return Scale;
	}

	inline void SetScale(const glm::mat4 & NewScale)
	{
		Scale = NewScale;
	}

	inline void SetRotate(const glm::mat4 & NewRotate)
	{
		Rotate = NewRotate;
	}

	inline void SetTranslate(const glm::mat4 & NewTranslate)
	{
		Translate = NewTranslate;
	}

	inline glm::mat4 GetWorld() const
	{
		return Scale * Translate * Rotate;
	}

private:
	glm::mat4 Scale;
	glm::mat4 Translate;
	glm::mat4 Rotate;
};

