#pragma once
#include "GameObjects/Object.h"
class TexturedGameObject :
	public Object
{
public:
	TexturedGameObject();

	void Start() override;
	void Update(float DeltaTime) override;
	void End() override;


};

