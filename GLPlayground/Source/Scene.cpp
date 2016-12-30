#include "Scene.h"
#include "RenderableScene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Renderable.h"
#include "ShaderManager.h"
#include "Logger.h"
#include "Transfrom.h"

Scene::Scene(RenderableScene & RenderScene) : RenderScene(RenderScene)
{

}


void Scene::Init()
{

	Material mt;

	mt.DiffuseTexture = std::make_unique<Texture>();
	
	std::string txStr = ResourceManager::GetTexturesFolder();
	mt.DiffuseTexture->LoadFromFile(txStr += "test.jpg");

	size_t ShaderProgramId = 0;
	
	if (!ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", ShaderProgramId))
	{
		Logger::GetLogger().LogString("Unable to create shader base", LogType::ERROR);
		return;
	}

	mt.Program = ShaderProgramId;

	//

	std::shared_ptr<Mesh> mesh(new Mesh(
	{  //Vertices
		{ glm::vec3(1.f, 1.f, 0.0f), glm::vec4(0, 0, 1, 1), glm::vec2(1,0) }, //0
		{ glm::vec3(1.f,  -1.f, 0.0f), glm::vec4(0, 1, 0, 1), glm::vec2(1,1) },  //1
		{ glm::vec3(-1.f, 1.f, 0.0f), glm::vec4(1, 0, 0, 1), glm::vec2(0,0) }, //2
		{ glm::vec3(-1.f,   -1.f, 0.0f), glm::vec4(0, 0, 1, 1), glm::vec2(0,1) }  //3
	},
		//Indices
	{
		0,
		1,
		2,
		2,
		1,
		3
	},

	std::move(mt)
	));
	//

	//RenderScene.AddMesh(mesh);


	std::unique_ptr<GameObject> gm = std::make_unique<GameObject>();

	gm->SetLogicScene(this);

	ComponentLocation rendLoc = gm->AddComponent(std::make_unique<Renderable>());

	Renderable* rend = (Renderable *)gm->GetComponentAtLocation(rendLoc);
	rend->SetMesh(mesh);

	ComponentLocation transfLoc = gm->AddComponent(std::make_unique<Transfrom>());

	Transfrom* transf = static_cast<Transfrom*>(gm->GetComponentAtLocation(transfLoc));

	GameObjects.push_back(std::move(gm));
	
	for (auto & gameObject : GameObjects)
	{
		gameObject->Start();
	}
}

void Scene::Update()
{
	for (auto & gameObject : GameObjects)
	{
		gameObject->Update(0);
	}
}

void Scene::DeInit()
{
	for (auto & gameObject : GameObjects)
	{
		gameObject->End();
	}
}

Scene::~Scene()
{
}
