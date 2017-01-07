#pragma once
#include "Mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

struct WindowInfo
{
	int Width;
	int Height;
};

class GLRenderer
{
public:
	GLRenderer();

	bool Initialize(GLFWwindow * Window);

	void Clear();
	void Present();
	void DrawMesh(const Mesh & mesh);
	void Draw();

	~GLRenderer();

	inline void GetCurrentWindowInfo(WindowInfo & info) const
	{
		glfwGetWindowSize(Context, &info.Width, &info.Height);
	}

private:
	GLFWwindow * Context;
};

