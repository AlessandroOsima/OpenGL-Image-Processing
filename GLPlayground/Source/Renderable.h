#pragma once
#include "Component.h"
#include "Mesh.h"
#include <memory>
#include "RenderableScene.h"
#include "Scene.h"

class Scene;

class Renderable :
	public Component
{
public:
	Renderable();
	virtual ~Renderable();

	virtual void Start() override;
	virtual void Update(float DeltaTime) override;
	virtual void LateUpdate(float DeltaTime) override;
	virtual void End() override;

	virtual ComponentsType GetType()
	{
		return ComponentsType::Renderable;
	}

	std::shared_ptr<Mesh> GetMesh()
	{
		return Mesh;
	}

	void SetMesh(std::shared_ptr<Mesh> NewMesh)
	{
		if (!LogicScene)
		{
			return;
		}

		Mesh = NewMesh;

		if (MeshIsRendering)
		{
			LogicScene->GetRenderScene().RemoveMesh(Loc);
		}

		Loc = LogicScene->GetRenderScene().AddMesh(Mesh);
		MeshIsRendering = true;
	}


private:
	std::shared_ptr<Mesh> Mesh;

	RenderableMeshLocation Loc;
	bool MeshIsRendering = false;
};

