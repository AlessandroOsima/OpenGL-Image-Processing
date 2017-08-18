#pragma once
#include "GameObjects/Object.h"
class TexturedObject :
	public Object
{
public:
	TexturedObject();

	void Start() override;
	void Update(float DeltaTime) override;
	void End() override;


};

