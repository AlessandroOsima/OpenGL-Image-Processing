#pragma once
#include "GameObjects/Component.h"
#include "glm/gtc/matrix_transform.hpp"

class Text :
	public Component
{
public:
	Text();
	
	bool SetText(const std::string & Text, const glm::vec4 & Color, const glm::vec3 & Position,  const std::string & Font, bool Infinite = false, bool PositionIsRelative = true, float Duration = -1);
	
	virtual ComponentsType GetType()
	{
		return ComponentsType::Text;
	}

	void Start() override;
	void Update(float DeltaTime) override;
	void LateUpdate(float DeltaTime) override;
	void End() override;
	
	~Text();
};

