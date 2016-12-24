#pragma once
#include "Mesh.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

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

private:
	GLFWwindow * Context;
};

