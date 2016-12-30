#pragma once
#include "RenderableScene.h"
#include "GameObject.h"

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
	std::vector<std::unique_ptr<GameObject>> GameObjects;
};

