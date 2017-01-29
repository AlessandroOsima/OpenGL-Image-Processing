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
#include "Managers/TextureManager.h"

Scene::Scene(RenderableScene & RenderScene) : RenderScene(RenderScene)
{

}


void Scene::Init()
{
	/*
	std::unique_ptr<Object> orginal = std::make_unique<TexturedGameObject>("base", "test.jpg");
	orginal->SetLogicScene(this);
	GameObjects.push_back(std::move(orginal));

	std::unique_ptr<Object> blurred = std::make_unique<TexturedGameObject>("box", "test.jpg");
	blurred->SetLogicScene(this);
	GameObjects.push_back(std::move(blurred));

	std::unique_ptr<Object> sharpened = std::make_unique<TexturedGameObject>("box", "test.jpg");
	sharpened->SetLogicScene(this);
	GameObjects.push_back(std::move(sharpened));

	std::unique_ptr<Object> median = std::make_unique<TexturedGameObject>("sobel", "test.jpg");
	median->SetLogicScene(this);
	GameObjects.push_back(std::move(median));

	
	for (auto & gameObject : GameObjects)
	{
		gameObject->Start();
	}

	bool Found = false;

	TextureInfo info = TextureManager::GetTextureManager().GetTextureFromName("test.jpg", Found).GetTextureInfo();

	if (!Found)
	{
		Logger::GetLogger().LogString("Unable to find texture info for test.jpg", LogType::ERROR);
		return;
	}

	float sizeX = info.Width;
	float sizeY = info.Height;

	//ORIGINAL
	Transform * originalTransform = static_cast<Transform*>(GameObjects[0]->GetComponentOfType(ComponentsType::Transform));
	originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX / 2, sizeY + sizeY / 2, 0)));
	

	//BLUR
	Transform * blurredTransform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));
	blurredTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX + sizeX / 2, sizeY + sizeY / 2, 0)));
	
	Renderable * blurredRenderable = static_cast<Renderable*>(GameObjects[1]->GetComponentOfType(ComponentsType::Renderable));
	UniformTypeData blurredUniformData { Filters::GenerateSmoothingMatrix() };
	UniformsToBind blurredUniform{ "Mask", blurredUniformData, UniformType::Mat3 };
	blurredRenderable->GetMesh()->AddUniform(blurredUniform);

	//SHARPEN
	Transform * sharpenedTransform = static_cast<Transform*>(GameObjects[2]->GetComponentOfType(ComponentsType::Transform));
	sharpenedTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX / 2, sizeY / 2, 0)));

	Renderable * sharpenedRenderable = static_cast<Renderable*>(GameObjects[2]->GetComponentOfType(ComponentsType::Renderable));
	UniformTypeData sharpenedUniformData { Filters::GenerateSharpenMatrix(9) };
	UniformsToBind sharpenedUniform{ "Mask", sharpenedUniformData, UniformType::Mat3 };
	sharpenedRenderable->GetMesh()->AddUniform(sharpenedUniform);

	//MEDIAN
	Transform * medianTransform = static_cast<Transform*>(GameObjects[3]->GetComponentOfType(ComponentsType::Transform));
	medianTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX + sizeX / 2, sizeY / 2, 0)));
	*/

	std::unique_ptr<Object> orginal = std::make_unique<TexturedGameObject>();
	orginal->SetLogicScene(this);
	GameObjects.push_back(std::move(orginal));

	std::unique_ptr<Object> filtered = std::make_unique<TexturedGameObject>();
	filtered->SetLogicScene(this);
	GameObjects.push_back(std::move(filtered));

	for (auto & gameObject : GameObjects)
	{
		gameObject->Start();
	}

	bool Found = false;

	size_t textureID;

	if (!TextureManager::GetTextureManager().CreateTextureFromFile("test.jpg", textureID))
	{
		Logger::GetLogger().LogString("Unable to find texture info for test.jpg", LogType::ERROR);
		return;
	}

	TextureInfo info = TextureManager::GetTextureManager().GetTextureFromID(textureID, Found).GetTextureInfo();

	float sizeX = info.Width;
	float sizeY = info.Height;

	//ORIGINAL
	Transform * originalTransform = static_cast<Transform*>(GameObjects[0]->GetComponentOfType(ComponentsType::Transform));
	originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX / 4, sizeY / 2, 0)));

	originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(info.Width / 4, info.Height / 4, 1.f)));

	//FILTERED
	Transform * filteredTransform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));
	filteredTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(sizeX / 4 + sizeX / 2, sizeY / 2, 0)));

	filteredTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(info.Width / 4, info.Height / 4, 1.f)));

	size_t BaseHash;
	size_t GrayscaleHash;
	size_t BoxHash;
	size_t SobelHash;


	if (ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", BaseHash) && ShaderManager::GetShaderManager().CreateShader("grayscale", "grayscale.vs", "grayscale.fs", GrayscaleHash) && ShaderManager::GetShaderManager().CreateShader("box", "box.vs", "box.fs", BoxHash) &&  ShaderManager::GetShaderManager().CreateShader("sobel", "sobel.vs", "sobel.fs", SobelHash))
	{

		RenderPassGroup originalPassGroup(sizeX, sizeY);

		Material passBaseMaterial{ textureID, BaseHash };
		RenderPass passBase(passBaseMaterial, true, true);
		originalPassGroup.RenderPasses.push_back(std::move(passBase));

		Renderable * originalRenderable = static_cast<Renderable*>(GameObjects[0]->GetComponentOfType(ComponentsType::Renderable));
		originalRenderable->AddPassesOnMesh(std::move(originalPassGroup));



		RenderPassGroup passGroup(sizeX, sizeY);

		Material passGrayMaterial{ textureID, GrayscaleHash };
		RenderPass passGray(passGrayMaterial, false, true);
		passGroup.RenderPasses.push_back(std::move(passGray));

		Material passSmoothMaterial{ textureID, BoxHash };
		RenderPass passSmooth(passSmoothMaterial, false, true);
		UniformTypeData smoothUniformData{ Filters::GenerateSmoothingMatrix() };
		UniformsToBind smoothUniform{ "Mask", smoothUniformData, UniformType::Mat3 };
		passSmooth.AddUniform(smoothUniform);
		passGroup.RenderPasses.push_back(std::move(passSmooth));

		Material passSharpenMaterial{ textureID, BoxHash };
		RenderPass passSharpen(passSharpenMaterial, false, true);
		UniformTypeData sharpenUniformData{ Filters::GenerateSharpenMatrix(8) };
		UniformsToBind sharpenUniform{ "Mask", sharpenUniformData, UniformType::Mat3 };
		passSharpen.AddUniform(sharpenUniform);
		passGroup.RenderPasses.push_back(std::move(passSharpen));

		Material passSobelMaterial{ textureID, SobelHash };
		RenderPass passSobel(passSobelMaterial, true, true);
		passGroup.RenderPasses.push_back(std::move(passSobel));

		Renderable * filteredRenderable = static_cast<Renderable*>(GameObjects[1]->GetComponentOfType(ComponentsType::Renderable));
		filteredRenderable->AddPassesOnMesh(std::move(passGroup));

	}


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
