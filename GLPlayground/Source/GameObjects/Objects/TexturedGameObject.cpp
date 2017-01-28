#include "TexturedGameObject.h"
#include "Renderer/RenderableScene.h"
#include "Managers/ResourceManager.h"
#include "Managers/TextureManager.h"
#include "Renderer/Texture.h"
#include "GameObjects/Components/Renderable.h"
#include "Managers/ShaderManager.h"
#include "Logger/Logger.h"
#include "GameObjects/Components/Transform.h"
#include "GameObjects/Objects/TexturedGameObject.h"
#include <glm/gtc/matrix_transform.hpp>

TexturedGameObject::TexturedGameObject(const std::string & ShaderName, const std::string & TextureName) : ShaderName(ShaderName), TextureName(TextureName)
{

}

void TexturedGameObject::Start()
{

	Object::Start();

	size_t TextureID = 0;
	
	if (!TextureManager::GetTextureManager().CreateTextureFromFile(TextureName, TextureID))
	{
		Logger::GetLogger().LogString("Unable to create texture", LogType::ERROR);
		return;
	}

	size_t ShaderProgramId = 0;

	if (!ShaderManager::GetShaderManager().CreateShader(ShaderName, ShaderName + ".vs", ShaderName + ".fs", ShaderProgramId))
	{
		Logger::GetLogger().LogString("Unable to create shader", LogType::ERROR);
		return;
	}

	Material mt(TextureID, ShaderProgramId);
	//

	std::shared_ptr<Mesh> mesh(new Mesh(
	{  //Vertices
		{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1,0) }, //0
		{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,1) },  //1
		{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,0) }, //2
		{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,1) }  //3
	},
		//Indices
	{
		0,
		1,
		2,
		2,
		1,
		3
	}
	));
	//

	ComponentLocation rendLoc = AddComponent(std::make_unique<Renderable>());

	Renderable* rend = (Renderable *)GetComponentAtLocation(rendLoc);
	rend->SetMesh(mesh);

	ComponentLocation transfLoc = AddComponent(std::make_unique<Transform>());

	Transform* transf = static_cast<Transform*>(GetComponentAtLocation(transfLoc));

	bool Found = false;
	Texture & tx = TextureManager::GetTextureManager().GetTextureFromID(TextureID, Found);

	transf->SetScale(glm::scale(glm::mat4(), glm::vec3(tx.GetTextureInfo().Width / 2, tx.GetTextureInfo().Height / 2, 1.f)));
}

void TexturedGameObject::Update(float DeltaTime)
{
	Object::Update(DeltaTime);
}

void TexturedGameObject::End()
{
	Object::End();
}

