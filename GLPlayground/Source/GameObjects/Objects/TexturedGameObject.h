#pragma once
#include "GameObjects/Object.h"
class TexturedGameObject :
	public Object
{
public:
	TexturedGameObject(const std::string & ShaderName, const std::string & TextureName);

	void Start() override;
	void Update(float DeltaTime) override;
	void End() override;

private:
	std::string ShaderName;
	std::string TextureName;
};

