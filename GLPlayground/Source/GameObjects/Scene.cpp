#include "stdafx.h"
#include "Scene.h"
#include "Renderer/RenderableScene.h"
#include "Managers/ResourceManager.h"
#include "Renderer/Texture.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/ShaderManager.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "GameObjects/Objects/TexturedObject.h"
#include <glm/gtc/matrix_transform.hpp>
#include "Math/Filters.h"
#include "Managers/TextureManager.h"
#include "GameObjects/Components/Text.h"


Scene::Scene(RenderableScene & RenderScene) : RenderScene(RenderScene)
{

}


void Scene::Init()
{
	std::unique_ptr<Object> orginal = std::make_unique<TexturedObject>();
	orginal->SetLogicScene(this);
	GameObjects.push_back(std::move(orginal));

	std::unique_ptr<Object> sharpen = std::make_unique<TexturedObject>();
	sharpen->SetLogicScene(this);
	GameObjects.push_back(std::move(sharpen));

	std::unique_ptr<Object> smooth = std::make_unique<TexturedObject>();
	smooth->SetLogicScene(this);
	GameObjects.push_back(std::move(smooth));

	std::unique_ptr<Object> sobel = std::make_unique<TexturedObject>();
	sobel->SetLogicScene(this);
	GameObjects.push_back(std::move(sobel));

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

	int sizeX = info.Width;
	int sizeY = info.Height;

	int inWindowSizeX = sizeX / 4;
	int inWindowSizeY = sizeY / 4;

	//ORIGINAL
	Transform * originalTransform = static_cast<Transform*>(GameObjects[0]->GetComponentOfType(ComponentsType::Transform));
	originalTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(inWindowSizeX, sizeY / 2 + inWindowSizeY, 0)));

	originalTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(inWindowSizeX, inWindowSizeY, 1.f)));

	//SHARPEN
	Transform * sharpenTransform = static_cast<Transform*>(GameObjects[1]->GetComponentOfType(ComponentsType::Transform));
	sharpenTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(inWindowSizeX + sizeX / 2, sizeY / 2 + inWindowSizeY, 0)));

	sharpenTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(inWindowSizeX, inWindowSizeY, 1.f)));

	//SMOOTH
	Transform * smoothTransform = static_cast<Transform*>(GameObjects[2]->GetComponentOfType(ComponentsType::Transform));
	smoothTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(inWindowSizeX, inWindowSizeY, 0)));

	smoothTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(inWindowSizeX, inWindowSizeY, 1.f)));

	//FILTERED
	Transform * filteredTransform = static_cast<Transform*>(GameObjects[3]->GetComponentOfType(ComponentsType::Transform));
	filteredTransform->SetTranslate(glm::translate(glm::mat4(), glm::vec3(inWindowSizeX + sizeX / 2, inWindowSizeY, 0)));

	filteredTransform->SetScale(glm::scale(glm::mat4(), glm::vec3(inWindowSizeX, inWindowSizeY, 1.f)));

	size_t BaseHash;
	size_t GrayscaleHash;
	size_t BoxHash;
	size_t SobelHash;


	if (ShaderManager::GetShaderManager().CreateShader("base", "base.vs", "base.fs", BaseHash) && ShaderManager::GetShaderManager().CreateShader("grayscale", "grayscale.vs", "grayscale.fs", GrayscaleHash) && ShaderManager::GetShaderManager().CreateShader("box", "box.vs", "box.fs", BoxHash) &&  ShaderManager::GetShaderManager().CreateShader("sobel", "sobel.vs", "sobel.fs", SobelHash))
	{
		//Set Up Renderpasses on Objects
		{
			RenderPassGroup originalPassGroup(sizeX, sizeY);

			Material passBaseMaterial{ textureID, BaseHash };
			RenderPass passBase(std::move(passBaseMaterial), true, true);
			originalPassGroup.RenderPasses.push_back(std::move(passBase));

			Renderable * originalRenderable = static_cast<Renderable*>(GameObjects[0]->GetComponentOfType(ComponentsType::Renderable));
			originalRenderable->AddPassesOnMesh(std::move(originalPassGroup));

			Text * originalText = static_cast<Text*>(GameObjects[0]->GetComponentOfType(ComponentsType::Text));
			originalText->SetText("ORIGINAL", glm::vec4(0, 0.6f, 1, 1), glm::vec3(-50, inWindowSizeY - 25, -0.1f), "arial.ttf", true, true);

		}

		{
			RenderPassGroup sharpenPassGroup(sizeX, sizeY);

			Material passSharpMaterial{ textureID, BoxHash };
			RenderPass passSharp(std::move(passSharpMaterial), true, true);
			UniformTypeData sharpUniformData{ Filters::GenerateSharpenFactor5() };
			UniformsToBind sharpUniform{ "Mask", sharpUniformData, UniformType::Mat3 };
			passSharp.AddUniform(sharpUniform);

			sharpenPassGroup.RenderPasses.push_back(std::move(passSharp));

			Renderable * sharpenRenderable = static_cast<Renderable*>(GameObjects[1]->GetComponentOfType(ComponentsType::Renderable));
			sharpenRenderable->AddPassesOnMesh(std::move(sharpenPassGroup));

			Text * sharpenText = static_cast<Text*>(GameObjects[1]->GetComponentOfType(ComponentsType::Text));
			sharpenText->SetText("GRAYSCALE + SHARPEN 3x3 FACTOR 5", glm::vec4(0, 0.6f, 1, 1), glm::vec3(-280, inWindowSizeY - 25, -0.1f), "arial.ttf", true, true);

		} 
		
		{
			RenderPassGroup passGroup(sizeX, sizeY);

			Material passSmoothMaterial{ textureID, BoxHash };
			RenderPass passSmooth(std::move(passSmoothMaterial), true, true);
			UniformTypeData smoothUniformData{ Filters::GenerateSmoothingMatrix() };
			UniformsToBind smoothUniform{ "Mask", smoothUniformData, UniformType::Mat3 };
			passSmooth.AddUniform(smoothUniform);
			passGroup.RenderPasses.push_back(std::move(passSmooth));

			Renderable * renderable = static_cast<Renderable*>(GameObjects[2]->GetComponentOfType(ComponentsType::Renderable));
			renderable->AddPassesOnMesh(std::move(passGroup));

			Text * smoothText = static_cast<Text*>(GameObjects[2]->GetComponentOfType(ComponentsType::Text));
			smoothText->SetText("BLUR 3x3", glm::vec4(0, 0.6f, 1, 1), glm::vec3(-50, inWindowSizeY - 25, -0.1f), "arial.ttf", true, true);

		}

		{
			RenderPassGroup passGroup(sizeX, sizeY);

			Material passGrayMaterial{ textureID, GrayscaleHash };
			RenderPass passGray(std::move(passGrayMaterial), false, true);
			passGroup.RenderPasses.push_back(std::move(passGray));

			Material passSmoothMaterial{ textureID, BoxHash };
			RenderPass passSmooth(std::move(passSmoothMaterial), false, true);
			UniformTypeData smoothUniformData{ Filters::GenerateSmoothingMatrix() };
			UniformsToBind smoothUniform{ "Mask", smoothUniformData, UniformType::Mat3 };
			passSmooth.AddUniform(smoothUniform);
			passGroup.RenderPasses.push_back(std::move(passSmooth));

			Material passSobelMaterial{ textureID, SobelHash };
			RenderPass passSobel(std::move(passSobelMaterial), true, true);
			passGroup.RenderPasses.push_back(std::move(passSobel));

			Renderable * filteredRenderable = static_cast<Renderable*>(GameObjects[3]->GetComponentOfType(ComponentsType::Renderable));
			filteredRenderable->AddPassesOnMesh(std::move(passGroup));

			Text * filteredText = static_cast<Text*>(GameObjects[3]->GetComponentOfType(ComponentsType::Text));
			filteredText->SetText("GRAYSCALE + BLUR 3x3 + SOBEL", glm::vec4(0, 0.6f, 1, 1), glm::vec3(-250, inWindowSizeY - 25, -0.1f), "arial.ttf", true, true);
		}
	}


}

void Scene::Update(float DeltaTime)
{
	for (auto & gameObject : GameObjects)
	{
		gameObject->Update(DeltaTime);
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
