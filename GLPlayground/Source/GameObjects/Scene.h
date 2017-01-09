#pragma once
#include "Renderer/RenderableScene.h"
#include "Object.h"

class Scene
{
public:
	Scene(RenderableScene & RenderScene);
	void Init();
	void Update();
	void DeInit();

	inline RenderableScene & GetRenderScene() const
	{
		return RenderScene;
	}

	~Scene();

private:
	RenderableScene & RenderScene;
	std::vector<std::unique_ptr<Object>> GameObjects;
};

