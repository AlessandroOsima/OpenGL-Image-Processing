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

TexturedGameObject::TexturedGameObject()
{

}

void TexturedGameObject::Start()
{

	Object::Start();
	//

	std::shared_ptr<Mesh> mesh(new Mesh(
	{  //Vertices
		{ glm::vec3(1.f, 1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(1,1) }, //0
		{ glm::vec3(1.f,  -1.f, -0.1f), glm::vec4(0, 1, 0, 1), glm::vec2(1,0) },  //1
		{ glm::vec3(-1.f, 1.f, -0.1f), glm::vec4(1, 0, 0, 1), glm::vec2(0,1) }, //2
		{ glm::vec3(-1.f,   -1.f, -0.1f), glm::vec4(0, 0, 1, 1), glm::vec2(0,0) }  //3
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
}

void TexturedGameObject::Update(float DeltaTime)
{
	Object::Update(DeltaTime);
}

void TexturedGameObject::End()
{
	Object::End();
}

