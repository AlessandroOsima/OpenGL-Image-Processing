// GLPlayground.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "GLRenderer.h"
#include "ShaderProgram.h"
#include "Mesh.h"
#include <GLFW/glfw3.h>
#include <string>
#include <iostream>
#include <strstream>
#include "Logger.h"
#include "ResourceManager.h"
#include "Texture.h"
#include "GLUtilities.h"
#include "RenderableScene.h"
#include "Scene.h"

struct WindowInfo
{
	int Width;
	int Height;
};

struct Engine
{
	GLRenderer & Renderer;
	RenderableScene & RendererScene;
	Scene & LogicScene;
	ResourceManager & ResManager;
};

GLFWwindow * CreateWindow(const WindowInfo & Window, const std::string & WindowTitle)
{
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_DOUBLEBUFFER, GL_TRUE);

#ifdef GL_DEBUG
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif

	return glfwCreateWindow(Window.Width, Window.Height, WindowTitle.c_str(), nullptr, nullptr);

}

int main()
{
	Logger::GetLogger().LogOnFile(true);

	GLRenderer renderer;

	if (glfwInit() == GL_FALSE)
	{
		return -1;
	}


	WindowInfo windowInfo{ 800, 600 };
	GLFWwindow * window = CreateWindow(windowInfo, "OpenGL Renderer - Texturing Test Window");

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	if (!renderer.Initialize(window))
	{
		glfwTerminate();
		return -1;
	}


	RenderableScene renderScene(renderer);
	Scene scene(renderScene);

	scene.Init();
	renderScene.Initialize();

	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		scene.Update();
		renderScene.RenderScene();
	}

	renderScene.DeInitialize();
	scene.DeInit();

	glfwTerminate();

    return 0;
}

