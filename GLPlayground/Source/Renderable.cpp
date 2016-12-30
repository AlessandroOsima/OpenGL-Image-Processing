#include "Renderable.h"
#include "Logger.h"
#include "Transfrom.h"

Renderable::Renderable()
{
}


Renderable::~Renderable()
{
}

void Renderable::Start()
{

}

void Renderable::Update(float DeltaTime)
{
}

void Renderable::LateUpdate(float DeltaTime)
{
	assert(Owner);

	Component * cmp = Owner->GetComponentOfType(ComponentsType::Transform);

	if (cmp)
	{
		Transfrom * tr = static_cast<Transfrom *>(cmp);

		Mesh->SetModel(tr->GetWorld());
	}
	else
	{
		Logger::GetLogger().LogString("No transform component found in GameObject", LogType::ERROR);
	}
}

void Renderable::End()
{
	if (LogicScene && MeshIsRendering)
	{
		LogicScene->GetRenderScene().RemoveMesh(Loc);
	}
}


