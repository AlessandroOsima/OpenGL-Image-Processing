#include "Scene.h"
#include "RenderableScene.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "Renderable.h"
#include "ShaderManager.h"
#include "Logger.h"
#include "Transform.h"
#include <glm/gtc/matrix_transform.hpp>

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
		{ glm::vec3(800.f, 600.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1,0) }, //0
		{ glm::vec3(800.f,  -600.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,1) },  //1
		{ glm::vec3(-800.f, 600.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,0) }, //2
		{ glm::vec3(-800.f,   -600.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,1) }  //3
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

	ComponentLocation transfLoc = gm->AddComponent(std::make_unique<Transform>());

	Transform* transf = static_cast<Transform*>(gm->GetComponentAtLocation(transfLoc));

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
		Transform * tr = (Transform*)(gameObject->GetComponentOfType(ComponentsType::Transform));

		//tr->SetRotate(glm::rotate(glm::mat4(), 0.7f, glm::vec3(0, 0, 1)));
		tr->SetScale(glm::scale(glm::mat4(), glm::vec3(0.5f)));
		tr->SetTranslate(glm::translate(glm::mat4(), glm::vec3(-800, 600.0f, 0)));
		
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
