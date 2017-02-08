#include "stdafx.h"
#include "Text.h"
#include "Renderer/FontRenderer.h"
#include "Logger/Asserts.h"
#include "Renderer/Font.h"
#include "GameObjects/Scene.h"
#include "GameObjects/Components/Transform.h"
#include "glm/gtc/matrix_transform.hpp"

Text::Text()
{
}


bool Text::SetText(const std::string & Text, const glm::vec4 & Color, const glm::vec3 & Position, const std::string & Font, bool Infinite, bool PositionIsRelative , float Duration )
{
	if (LogicScene)
	{
		RenderableScene & renderableScene = LogicScene->GetRenderScene();

		size_t FontRendererID = renderableScene.CreateFontRenderer(Font);

		if (FontRendererID)
		{
			
			Transform * TransformComponent = static_cast<Transform*>(Owner->GetComponentOfType(ComponentsType::Transform));

			glm::vec4 FinalPosition = glm::vec4(Position.x, Position.y, Position.z, 1);

			if (TransformComponent && PositionIsRelative)
			{
				FinalPosition = TransformComponent->GetTranslate() * FinalPosition;
			}

			TextInfo info{Text, Color, FinalPosition};
			
			FontRenderer * fontRenderer = renderableScene.GetFontRenderer(FontRendererID);

			AssertWithMessage(fontRenderer, "No Font Renderer Found");

			fontRenderer->RenderText(info, Duration, Infinite);

			return true;
		}

		return false;
	}

	return false;
}

void Text::Start()
{

}

void Text::Update(float DeltaTime)
{

}

void Text::LateUpdate(float DeltaTime)
{

}

void Text::End()
{

}

Text::~Text()
{
}
