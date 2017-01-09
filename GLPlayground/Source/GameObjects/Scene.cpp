#include "Scene.h"
#include "Renderer/RenderableScene.h"
#include "Managers/ResourceManager.h"
#include "Renderer/Texture.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/ShaderManager.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "GameObjects/Objects/TexturedGameObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Math/Filters.h"

Scene::Scene(RenderableScene & RenderScene) : RenderScene(RenderScene)
{

}


void Scene::Init()
{

	std::unique_ptr<Object> orginal = std::make_unique<TexturedGameObject>("base", "test.jpg");
	orginal->SetLogicScene(this);
	GameObjects.push_back(std::move(orginal));

	std::unique_ptr<Object> blurred = std::make_unique<TexturedGameObject>("box", "test.jpg");
	blurred->SetLogicScene(this);
	GameObjects.push_back(std::move(blurred));

	std::unique_ptr<Object> sharpened = std::make_unique<TexturedGameObject>("box", "test.jpg");
	sharpened->SetLogicScene(this);
	GameObjects.push_back(std::move(sharpened));

	
	for (auto & gameObject : GameObjects)
	{
		gameObject->Start();
	}

	//ORIGINAL
	Transform * originalTransform = static_cast<Transform*>(GameObjects[0]->GetComponentOfType(ComponentsType::Transform));
	originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(300 / 2, 420.0f + 210.f, 0)));
	

	//BLUR
	Transform * blurredTransform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));
	blurredTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(450, 420.0f + 210.f, 0)));
	
	Renderable * blurredRenderable = static_cast<Renderable*>(GameObjects[1]->GetComponentOfType(ComponentsType::Renderable));
	UniformTypeData blurredUniformData { Filters::GenerateSmoothingMatrix() };
	UniformsToBind blurredUniform{ "Mask", blurredUniformData, UniformType::Mat3 };
	blurredRenderable->GetMesh()->AddUniform(blurredUniform);

	//SHARPEN
	Transform * sharpenedTransform = static_cast<Transform*>(GameObjects[2]->GetComponentOfType(ComponentsType::Transform));
	sharpenedTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(300 / 2, 420.0f / 2, 0)));

	Renderable * sharpenedRenderable = static_cast<Renderable*>(GameObjects[2]->GetComponentOfType(ComponentsType::Renderable));
	UniformTypeData sharpenedUniformData { Filters::GenerateSharpenMatrix(8) };
	UniformsToBind sharpenedUniform{ "Mask", sharpenedUniformData, UniformType::Mat3 };
	sharpenedRenderable->GetMesh()->AddUniform(sharpenedUniform);

}

void Scene::Update()
{
	for (auto & gameObject : GameObjects)
	{
		//Transform * tr = (Transform*)(gameObject->GetComponentOfType(ComponentsType::Transform));

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
